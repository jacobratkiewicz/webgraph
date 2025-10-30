/*               
 * Portions copyright (c) 2003-2007, Paolo Boldi and Sebastiano Vigna. Translation copyright (c) 2007, Jacob Ratkiewicz
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "../input_bitstream.hpp"
#include "../output_bitstream.hpp"

void test_write(unsigned char* what, int num_bytes);
void test_read(int num_bytes);

boost::shared_ptr<std::vector<unsigned char> > test_write_to_array( unsigned char* what, int num_bytes );
void test_read_from_array( boost::shared_ptr< std::vector<unsigned char> >& data, int num_bytes );

using namespace std;
using namespace webgraph;
using namespace boost;

int main(int, char**) {
	cerr << "Opening an output bitstream to dump some stuff.\n";
	cerr.setf( ios::hex );
	
	long somebits = 0xaabbccdd;	
	unsigned char* b = (unsigned char*)&somebits;
	
	test_write( b, sizeof(somebits) );
	
	test_read( sizeof( somebits ) );

	cerr << "Done with files.. now time for arrays.\n";

	boost::shared_ptr< vector<unsigned char> > res = test_write_to_array( b, sizeof(somebits) );
	
	cerr << "We've written .. now let's see how it looks.\n";
    cerr << "The bytes of res[] are " << endl;
	for( int i = 0; i < sizeof( somebits ); i++ )
		 cerr << (int)(*res)[i] << " ";
	
	test_read_from_array( res, sizeof(somebits) );

	return 0;
}

/**
 * Test writing
 */
void test_write(unsigned char* what, int num_bytes) {
	obitstream obs( "bits" );
	
	cerr << "The bytes of what[] are " << endl;
	for( int i = 0; i < num_bytes; i++ )
		 cerr << (int)what[i] << " ";
		 
	cerr << endl;
	
	obs.write( what, num_bytes * 8 );
}

/**
 * test reading
 */
void test_read(int num_bytes) {
	unsigned char* b = new unsigned char[num_bytes];
	
	ibitstream ibs( "bits" );
	
	ibs.read( b, num_bytes * 8);

	cerr << "The bytes of b[] are " << endl;
	for( int i = 0; i < num_bytes; i++ )
		 cerr << (int)b[i] << " ";
		 
	cerr << endl;
}

/**
 * test writing to array
 */
boost::shared_ptr< std::vector<unsigned char> > test_write_to_array( unsigned char* what, int num_bytes ) {
	boost::shared_ptr< std::vector<unsigned char> > ary( new std::vector<unsigned char>(num_bytes) );
	
	obitstream obs( ary );
	
	cerr << "The bytes of what[] are " << endl;
	for( int i = 0; i < num_bytes; i++ )
		 cerr << (int)what[i] << " ";
		 
	cerr << endl;
	
	obs.write( what, num_bytes * 8 );
	
	return ary;
}

/**
 * test reading from array
 */
void test_read_from_array( boost::shared_ptr< std::vector<unsigned char> >& data, int num_bytes ) {
	ibitstream ibs( data );
	
	unsigned char* b = new unsigned char[num_bytes];
	
	ibs.read( b, num_bytes * 8);

	cerr << "The bytes of b[] are " << endl;
	for( int i = 0; i < num_bytes; i++ )
		 cerr << (int)b[i] << " ";
		 
	cerr << endl;
}
