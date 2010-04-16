package br.ufg.inf.refman

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 07/04/2010
 * Time: 16:24:34
 * To change this template use File | Settings | File Templates.
 */
interface SiteDirector {
    public List collectPages(String url, Closure closure)
}
