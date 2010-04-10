package br.ufg.inf.refman

/**
 * Created by IntelliJ IDEA.
 * User: gustavo
 * Date: 10/04/2010
 * Time: 13:28:11
 * To change this template use File | Settings | File Templates.
 */
class Engine {
    String name
    String clientClassName

    public Class getClientClass() {
        clientClassName ? Class.forName(clientClassName, true, Thread.currentThread().contextClassLoader) : null
    }

    public void setClientClass(Class clientClass) {
        clientClassName = clientClass.name
    }

    public Client newClient() {
        clientClass.newInstance()
    }
}
