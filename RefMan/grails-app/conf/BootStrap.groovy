import br.ufg.inf.refman.Engine
import br.ufg.inf.refman.google.GoogleScholarClient
import br.ufg.inf.refman.acm.ACMClient

class BootStrap {

     def init = { servletContext ->
         new Engine(name: 'Google Scholar', clientClass: GoogleScholarClient).save()
         new Engine(name: 'ACM', clientClass: ACMClient).save()
     }
    
     def destroy = {
     }
} 