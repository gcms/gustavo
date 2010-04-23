package br.ufg.inf.pbs

class UtilsTagLib {
    def repeat = { attr, body ->
        int times = attr.times.toInteger()

        times.times {
            out << body()
        }
    }
}
