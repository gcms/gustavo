package br.ufg.inf.utils;

public class LevenshteinDistance {
    public static int calcule(String source, String target) {
      /*
        The difference between this impl. and the previous is that, rather
         than creating and retaining a matrix of size source.length()+1 by target.length()+1,
         we maintain two single-dimensional arrays of length source.length()+1.  The first, d,
         is the 'current working' distance array that maintains the newest distance cost
         counts as we iterate through the characters of String source.  Each time we increment
         the index of String target we are comparing, d is copied to p, the second int[].  Doing so
         allows us to retain the previous cost counts as required by the algorithm (taking
         the minimum of the cost count to the left, up one, and diagonally up and to the left
         of the current cost count being calculated).  (Note that the arrays aren'target really
         copied anymore, just switched...this is clearly much better than cloning an array
         or doing a System.arraycopy() each time  through the outer loop.)

         Effectively, the difference between the two implementations is this one does not
         cause an out of memory condition when calculating the LD over two very large strings.
      */

      int n = source.length(); // length of source
      int m = target.length(); // length of target

      if (n == 0) {
        return m;
      } else if (m == 0) {
        return n;
      }

      int p[] = new int[n+1]; //'previous' cost array, horizontally
      int d[] = new int[n+1]; // cost array, horizontally
      int _d[]; //placeholder to assist in swapping p and d

      // indexes into strings source and target
      int i; // iterates through source
      int j; // iterates through target

      char t_j; // jth character of target

      int cost; // cost

      for (i = 0; i<=n; i++) {
         p[i] = i;
      }

      for (j = 1; j<=m; j++) {
         t_j = target.charAt(j-1);
         d[0] = j;

         for (i=1; i<=n; i++) {
            cost = source.charAt(i-1)==t_j ? 0 : 1;
            // minimum of cell to the left+1, to the top+1, diagonally left and up +cost
            d[i] = Math.min(Math.min(d[i-1]+1, p[i]+1),  p[i-1]+cost);
         }

         // copy current distance counts to 'previous row' distance counts
         _d = p;
         p = d;
         d = _d;
      }

      // our last action in the above loop was to switch d and p, so p now
      // actually has the most recent cost counts
      return p[n];
    }
}
