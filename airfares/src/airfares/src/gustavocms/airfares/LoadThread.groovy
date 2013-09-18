package gustavocms.airfares

import gustavocms.airfares.SiteDirector
import gustavocms.airfares.query.fetch.ItineraryQuery

/**
 * Created with IntelliJ IDEA.
 * User: gustavosousa
 * Date: 05/09/13
 * Time: 12:30
 * To change this template use File | Settings | File Templates.
 */
class LoadThread extends Thread {
    SiteDirector director
    ItineraryQuery query

    List result

    public void run() {
        result = director.loadItineraries(query)
        print "."
    }
}