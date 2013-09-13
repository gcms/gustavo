package gustavocms.airfares
/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 12/09/13
 * Time: 12:52
 * To change this template use File | Settings | File Templates.
 */
class FareLoader implements Runnable {
    QueryService queryService

    private List<Query> queries
    private Thread thread
    private boolean ativo = false

    public FareLoader(QueryService queryService) {
        this.queryService = queryService
        queries = queryService.queries
    }


    public void stop() {
        if (!ativo)
            throw new IllegalStateException()

        ativo = false
    }

    public void run() {
        while (ativo) {
            println "while(ativo)"
            Query.list().each {
                if (!ativo)
                    return

//                println "checkList(${queries.size()})"
                checkList()
//                println "check(it)"
                check(it)
            }
        }
    }

    void checkList() {
        while (ativo && !queries.empty) {
            check(queries.remove(0))
        }
    }

    void check(Query query) {
        try {
            queryService.checkPrice(query)
        } catch (Exception ex) {
            ex.printStackTrace()
        }

    }

    public void start() {
        if (thread != null)
            throw new IllegalStateException()

        thread = new Thread(this)
        ativo = true
        thread.start()
    }
}
