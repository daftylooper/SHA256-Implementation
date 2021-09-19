#include<iostream>
#include<bitset>
#include<vector>
#include<fstream>

using namespace std;

bitset<32> SHR(bitset<32> x, int n); //Shift Right Operation
bitset<32> ROTR(bitset<32> x, int n); //Rotate Right Operation
bitset<32> XOR(bitset<32> x, bitset<32> y); //Exclusively OR Operation
bitset<32> ADD(bitset<32> x, bitset<32> y); //Adds two binary numbers
bitset<32> sigma0(bitset<32> x); //A compound function using XOR, ROTR, SHR
bitset<32> sigma1(bitset<32> x); //A compound function using XOR, ROTR, SHR
bitset<32> theta0(bitset<32> x); //A compound function using XOR, ROTR
bitset<32> theta1(bitset<32> x); //A compound function using XOR, ROTR
bitset<32> choice(bitset<32> x, bitset<32> y, bitset<32> z);
bitset<32> majority(bitset<32> x, bitset<32> y, bitset<32> z);
string toBinary(string message);
bitset<512> makeMessage(string input); //Creates a 512 bit bitset
vector< bitset<32>> makeMessageSchedule(bitset<512> message); //Creates a array of 64, 32bit numbers of the "message"
vector<bitset<32>> makeConstants(); //Creates 64 constants based on some rule
vector<bitset<32>> makeH0(); //Creates another set of 8 constants
vector<bitset<32>> makeH1(vector<bitset<32>> H0, vector<bitset<32>> constants, vector<bitset<32>> messageSchedule);
vector<bitset<32>> output(vector<bitset<32>> H0, vector<bitset<32>> H1);
vector<bitset<32>> makeOutput();

int main(){

	vector<bitset<32>> hashedOutput = makeOutput();
	for(int i=0;i<8; i++){
		cout<<hashedOutput[i];
	}

	return 0;
}

vector<bitset<32>> makeOutput(){
	cout<<"Enter your string......\n";
	string input;
	cin>>input;

	//If the length(binary(message)) > 512-64-1, make another message block( basically, vector<bitset<512>> )
	//For a message with length equal to 512-64-1, is the length of the message in binary =< 64?

	bitset<512> message; //creates a 512 bit bitset
	message = makeMessage(input); //refactors it
	
	vector<bitset<32>> messageSchedule;
	messageSchedule = makeMessageSchedule(message); //creates a array of 64, 32bit numbers of the "message"
	messageSchedule.shrink_to_fit(); //Shrinks vector size to what it truly is, to save memory

	vector<bitset<32>> constants;
	constants = makeConstants();
	constants.shrink_to_fit();

	vector<bitset<32>> H0;
	H0 = makeH0();
	H0.shrink_to_fit();
	
	vector<bitset<32>> H1;
	H1 = makeH1(H0, constants, messageSchedule);
	H1.shrink_to_fit();

	vector<bitset<32>> hashedOutput;
	hashedOutput = output(H0, H1);
	hashedOutput.shrink_to_fit();

	return hashedOutput;
}

vector<bitset<32>> output(vector<bitset<32>> H0, vector<bitset<32>> H1){
	vector<bitset<32>> hashedOutput;
	for(int i=0 ;i<8; i++){
		hashedOutput.push_back(ADD(H0[i], H1[i]));
	}
	return hashedOutput;
}

vector<bitset<32>> makeH1(vector<bitset<32>> H0, vector<bitset<32>> constants, vector<bitset<32>> messageSchedule){

	vector<bitset<32>> H1 = H0;
	bitset<32> T1;
	bitset<32> T2;

	for(int i=0; i<64; i++){
	T1 = ADD(theta1(H1[4]), ADD(choice(H1[4], H1[5], H1[6]), ADD(H1[7], ADD(constants[i], messageSchedule[i]))));
	T2 = ADD(theta0(H1[0]), majority(H1[0], H1[1], H1[2]));

	//cout<<T1<<"\n"<<T2<<"\n\n";

	H1[7] = H1[6];
	H1[6] = H1[5];
	H1[5] = H1[4];
	H1[4] = ADD(H1[3], T1);
	H1[3] = H1[2];
	H1[2] = H1[1];
	H1[1] = H1[0];
	H1[0] = ADD(T1, T2);
	}

	return H1;
}

vector<bitset<32>> makeH0(){
	vector<bitset<32>> H0 = {};
	const unsigned int H0_k[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
	for(int i=0; i<8; i++){
		H0.push_back(bitset<32>(H0_k[i]));
	}
	return H0;
}

vector<bitset<32>> makeConstants(){
	vector<bitset<32>> constants = {};
	
	const unsigned int sha256_k[64] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

    for(int i=0; i<64; i++){
    	constants.push_back(bitset<32>(sha256_k[i]));
    }

    return constants;
}

vector<bitset<32>> makeMessageSchedule(bitset<512> message){

	vector<bitset<32>> messageSchedule = {};

	int len = 512;
	int set = 32;
	int count = len - 1;
	string messageSubset = "";

	//Creates 16 sets of binary numbers, each 32 bit long.
	//The <bitset> library assigns indices to each binary number in a set of binary, counter to the normal convention of assigning indices to arrays

	for(int i=0; i<16; i++){
		for(int j=count; j>(count-set); j--){
			messageSubset+=to_string(message[j]);
		}
		messageSchedule.push_back(bitset<32>(messageSubset));
		messageSubset = "";
		count-=set;
	}

	bitset<32> msg;

	for(int i=16; i<64; i++){
		msg = ADD(ADD(messageSchedule[i-16], sigma0(messageSchedule[i-15])), ADD(messageSchedule[i-7], sigma1(messageSchedule[i-2])));
		messageSchedule.push_back(msg);
	}

	return messageSchedule;
}

bitset<512> makeMessage(string input){
	string pad;
	int padlen = 512 - toBinary(input).length() - 1 - 64;
	for(int i=0; i<padlen; i++){
		pad+="0";
	}
	int msglen = toBinary(input).length();
	bitset<64> bmsglen(msglen);
	string strmsglen = bmsglen.to_string();
	bitset<512> message(toBinary(input) + "1" + pad + strmsglen);
	return message;
}

string toBinary(string message){
	string binaryString = "";
	for(int i=0; i<message.length(); i++){
		binaryString+=bitset<8>(message[i]).to_string();
	}
	return binaryString;
}

bitset<32> SHR(bitset<32> x, int n){
	return x>>n;
}

bitset<32> ROTR(bitset<32> x, int n){
	for(int i=0; i<n; i++){
		int r = x[0]; //In bitset<n> the 0th element is the right most and nth element is leftmost
		x = x>>1;
		x[31] = r;
	}	
	return x;
}

bitset<32> XOR(bitset<32> x, bitset<32> y){
	return x^y;
}

// Function to add two bitsets
bitset<32> ADD(bitset<32> x, bitset<32> y){ //Addition is done in mod 2^32, overflows are wrapped.
	unsigned xint = x.to_ulong();
	unsigned yint = y.to_ulong();
	unsigned foo = xint + yint;
	bitset<32> sum (foo);
	return sum;
}

bitset<32> sigma0(bitset<32> x){
	return XOR(XOR(ROTR(x, 7), ROTR(x, 18)), SHR(x, 3));
}

bitset<32> sigma1(bitset<32> x){
	return XOR(XOR(ROTR(x, 17), ROTR(x, 19)), SHR(x, 10));
}

bitset<32> theta0(bitset<32> x){
	return XOR(XOR(ROTR(x, 2), ROTR(x, 13)), ROTR(x, 22));
}

bitset<32> theta1(bitset<32> x){
	return XOR(XOR(ROTR(x, 6), ROTR(x, 11)), ROTR(x, 25));
}

bitset<32> choice(bitset<32> x, bitset<32> y, bitset<32> z){
	//vector<bool> vec;
	bitset<32> output;
	for(int i=0; i<32; i++){
		if(x[i] == 1){
			output[i] = y[i];
		}
		if(x[i] == 0){
			output[i] = z[i];
		}
	}
	return output;
}

bitset<32> majority(bitset<32> x, bitset<32> y, bitset<32> z){
	bitset<32> output;
	for(int i=0; i<32; i++){
	int sum = x[i] + y[i] + z[i]; 
	if(sum>=2 && sum<=3){
		output[i] = 1;
		}
	if(sum>=0 && sum<=1){
		output[i] = 0;
		}
	}
	
	return output;
}
