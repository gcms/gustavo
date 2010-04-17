import br.ufg.inf.refman.Engine
import br.ufg.inf.refman.google.GoogleScholarClient
import br.ufg.inf.refman.acm.ACMClient
import br.ufg.inf.refman.ieee.IEEEClient

class BootStrap {

     def init = { servletContext ->
         registerClient('Google Scholar', GoogleScholarClient)
         registerClient('ACM', ACMClient)
         registerClient('IEEE', IEEEClient)
     }
    
     private void registerClient(String name, Class clientClass) {
         if (!Engine.findByName(name))
            new Engine(name: name, clientClass: clientClass).save()
     } 
    
     def destroy = {
     }
} 