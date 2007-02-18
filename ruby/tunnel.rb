
class Request
	def initialize(message, *params)
		@message = message
		@params = params
	end

	attr_reader :message, :params
end

class Response
	def initialize(result = nil)
		@result = result
	end
	attr_reader :result
end



require 'socket'
require 'webrick'

class LocalServer
	def initialize(local_port, mount, host, port)
		@local_port = local_port
		@mount = mount
		@host = host
		@port = port
		@id_sock = {}
	end

	def open
		sock = TCPSocket.new(@host, @port)
		id = sock.fileno
		@id_sock[id] = sock
#		puts "adding @id_sock[#{id}] = #{sock}"
		id
	end

	def close(id)
#		puts "Removing #{id}..."
#		puts "@id_sock[#{id}] = #{@id_sock[id]}"
		@id_sock.delete(id).close
#		puts "@id_sock[#{id}] = #{@id_sock[id]}"
	rescue
	end

	def write(id, data)
		sock = @id_sock[id]
		sock.write(data)
	end

	def read_ids(ids)
		result = {}

		ios = @id_sock.empty? ? [] : @id_sock.values_at(*ids)

#		puts "@id_sock = #{@id_sock.inspect}"
#		puts "select ios = #{ios.inspect}"
		read_ios = (IO.select(ios, nil, nil, 0) || [[]])[0]
		read_ios.each do |io|
			begin
				data = io.recv_nonblock(8096)
				result[io.fileno] = data
				if data.empty?
					close(io.fileno)
				end
			rescue Errno::EAGAIN
			rescue Exception => e
				result[io.fileno] = e
				close(io.fileno)
			end
		end

		result
	end

	def read(id)
		read_ids([id])
	end

	def read_all
		read_ids(@id_sock.keys)
	end

	def parse_req(req, res)
		query = WEBrick::HTTPUtils.parse_query(req.body)
		request = Marshal.load(query['request'])

		Response.new(method(request.message).call(*request.params))
	rescue Exception => e
		Response.new(e)
	end
	private :parse_req


	def start
		s = WEBrick::HTTPServer.new(:Port => @local_port)

		s.mount_proc(@mount) do |req, res|
			res.body = Marshal.dump(parse_req(req, res))
		end

		s.start
	end
end



require 'uri'
require 'net/http'

class Proxy
	def initialize(url)
		@url = URI.parse(url)
	end

	def request(request)
		req = Net::HTTP::Post.new(@url.path)
		req.set_form_data({'request' => Marshal.dump(request)}, ';')

		res = Net::HTTP.new(@url.host, @url.port).start \
			{|http| http.request(req)}

		Marshal.load(res.body)
	end
	private :request

	def method_missing(name, *params)
#		puts "calling #{name}()"
		result = request(Request.new(name, *params)).result

		if result.is_a?(Exception)
			raise result
		end

		result
	end
end

class RemoteServer
	def initialize(url, port)
		@proxy = Proxy.new(url)
		@port = port
		@id_sock = {}
	end

	def start
		Thread.start {
			loop do
				begin
#					puts "@id_sock = #{@id_sock.inspect}"
					read = @id_sock.empty? ? {} \
						: @proxy.read_ids(@id_sock.keys)

					read.each do |id, data|
						if data.is_a?(Exception) or data.empty?
							@id_sock[id].close
							@id_sock.delete(id)
						else
							puts "data to   #{id}: #{data.length}"
							@id_sock[id].write(data)
						end
					end
				rescue Exception => e
#					puts "Shouldn't happen: #{e}"
#					puts "\t#{e.backtrace}"
				end
			end
		}

		dts = TCPServer.new(@port)
		puts "Receiving connections on localhost:#{@port}"

		loop do
			Thread.start(dts.accept) do |s|
				begin
					puts "\tReceived a connection from #{s.to_s}"
					id = @proxy.open
					puts "\tRemote id = #{id}"

					begin
						@id_sock[id] = s
						loop do
							begin
								data = s.recv_nonblock(8096)

								puts "data from #{id}: #{data.length}"
								if data.empty?
									@id_sock.delete(id)
									@proxy.close(id)
									s.close
									break
								else
									@proxy.write(id, data)
								end
							rescue Errno::EAGAIN
							rescue Errno::EWOULDBLOCK
							rescue Exception => e
#							puts "Error on #{id}: #{e}"
#							puts "\t#{e.class}"
								@proxy.close(id)
								raise
							end
						end
						rescue
							@id_sock.delete(id)
							raise
						end
				rescue
					s.close
				end
			end
		end

	end
end

=begin
def start(port)
	dts = TCPServer.new('localhost', port)

	loop do
		Thread.start(dts.accept) do |s|
		end
	end
end
=end
