package gustavocms.airfares.query.fetch

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 11/09/13
 * Time: 08:20
 * To change this template use File | Settings | File Templates.
 */
class ItineraryQueryRoute {
    String from
    String to
    Date day

    public String toString() {
        "${from}-${to} [${day.format('dd/MM/yyyy')}]"
    }
}
