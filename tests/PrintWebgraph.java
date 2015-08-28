import it.unimi.dsi.webgraph.*;
import it.unimi.dsi.fastutil.ints.*;

public class PrintWebgraph {

    public static void main( String[] args ) throws Exception {
        ImmutableGraph g;

        if( args.length < 1 ) {
            System.err.println( "Need to supply name of graph to print." );
            System.exit( 1 );
        }

        String graphName = args[0];

        g = ImmutableGraph.load( graphName );

        NodeIterator itor = g.nodeIterator();

        System.err.println( g.numNodes() );

        while( itor.hasNext() ) {

            int nodeID = itor.nextInt();
            //System.err.println( itor.nextInt() );
            System.err.println( nodeID );
            //itor.nextInt();
            IntIterator succItor = g.successors( nodeID ); //itor.successors();

            if( succItor.hasNext() ) 
                System.err.print( succItor.nextInt() );
            
            while( succItor.hasNext() ) {
                System.err.print( " " + succItor.nextInt() );
            }

            System.err.println();
        }
    }
}
