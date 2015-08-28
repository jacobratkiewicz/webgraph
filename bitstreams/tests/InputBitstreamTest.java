import it.unimi.dsi.mg4j.io.*;
import java.io.*;

/**
 * Test harness for output bitstream
 */
public class InputBitstreamTest {
    public static void main( String[] args ) throws Exception {
//        PrintStream ps = new PrintStream( new BufferedOutputStream( new FileOutputStream( "java_obs_log" )));

//        byte[] dat = new byte[800];

//        DebugOutputBitStream obs = new DebugOutputBitStream( new OutputBitStream( dat ), ps );
        InputBitStream ibs = new InputBitStream( args[1] );

        // Call a bunch of methods.
        assert 5 == ibs.readGamma();
        assert 0 == ibs.readUnary();
        assert 0 == ibs.readGamma();
        assert 44 == ibs.readZeta( 5 );
        assert 15 == ibs.readZeta( 5 );
        assert 33 == ibs.readZeta( 5 );
        assert 2 == ibs.readZeta( 5 );
        assert 12 == ibs.readZeta( 5 );
        assert 1 == ibs.readGamma();
        assert 0 == ibs.readUnary();
        assert 0 == ibs.readGamma();
        assert 168 == ibs.readZeta( 5 );
        assert 12 == ibs.readGamma();
        assert 0 == ibs.readUnary();
        assert 0 == ibs.readGamma();
        assert 6 == ibs.readZeta( 5 );
        assert 5 == ibs.readZeta( 5 );
        assert 19 == ibs.readZeta( 5 );
        assert 7 == ibs.readZeta( 5 );
        assert 7 == ibs.readZeta( 5 );
        assert 0 == ibs.readZeta( 5 );
        assert 5 == ibs.readZeta( 5 );
        assert 17 == ibs.readZeta( 5 );
        assert 5 == ibs.readZeta( 5 );
        assert 2 == ibs.readZeta( 5 );
        assert 14 == ibs.readZeta( 5 );
        assert 2 == ibs.readZeta( 5 );
        assert 17 == ibs.readGamma();
        assert 0 == ibs.readUnary();
        assert 0 == ibs.readGamma();
        assert 8 == ibs.readZeta( 5 );
        assert 0 == ibs.readZeta( 5 );
        assert 1 == ibs.readZeta( 5 );
        assert 4 == ibs.readZeta( 5 );
        assert 5 == ibs.readZeta( 5 );
        assert 1 == ibs.readZeta( 5 );
        assert 0 == ibs.readZeta( 5 );
        assert 16 == ibs.readZeta( 5 );
        assert 2 == ibs.readZeta( 5 );
        assert 0 == ibs.readZeta( 5 );
        assert 5 == ibs.readZeta( 5 );
        assert 0 == ibs.readZeta( 5 );
        assert 11 == ibs.readZeta( 5 );
        assert 16 == ibs.readZeta( 5 );
        assert 1 == ibs.readZeta( 5 );
        assert 11 == ibs.readZeta( 5 );
        assert 0 == ibs.readZeta( 5 );
        assert 15 == ibs.readGamma();
        assert 1 == ibs.readUnary();
        assert 8 == ibs.readGamma();
        assert 0 == ibs.readGamma();
        assert 3 == ibs.readGamma();
        assert 0 == ibs.readGamma();
        assert 0 == ibs.readGamma();
        assert 1 == ibs.readGamma();
        assert 0 == ibs.readGamma();
        assert 0 == ibs.readGamma();
        assert 5 == ibs.readGamma();
        assert 0 == ibs.readGamma();
        assert 4 == ibs.readZeta( 5 );
        assert 15 == ibs.readZeta( 5 );
        assert 4 == ibs.readZeta( 5 );
        assert 8 == ibs.readZeta( 5 );
        assert 2 == ibs.readZeta( 5 );
        assert 3 == ibs.readZeta( 5 );
        assert 28 == ibs.readZeta( 5 );
        assert 4 == ibs.readZeta( 5 );
        assert 6 == ibs.readZeta( 5 );
        assert 6 == ibs.readZeta( 5 );
        assert 13 == ibs.readGamma();
        assert 0 == ibs.readUnary();
        assert 0 == ibs.readGamma();
        assert 7 == ibs.readZeta( 5 );
        assert 6 == ibs.readZeta( 5 );
        assert 1 == ibs.readZeta( 5 );
        assert 0 == ibs.readZeta( 5 );
        assert 5 == ibs.readZeta( 5 );
        assert 0 == ibs.readZeta( 5 );
        assert 32 == ibs.readZeta( 5 );
        assert 2 == ibs.readZeta( 5 );
        assert 4 == ibs.readZeta( 5 );
        assert 12 == ibs.readZeta( 5 );
        assert 14 == ibs.readZeta( 5 );
        assert 3 == ibs.readZeta( 5 );
        assert 4 == ibs.readZeta( 5 );
        assert 10 == ibs.readGamma();
        assert 0 == ibs.readUnary();
        assert 0 == ibs.readGamma();
        assert 12 == ibs.readZeta( 5 );
        assert 4 == ibs.readZeta( 5 );
        assert 17 == ibs.readZeta( 5 );
    }
}
