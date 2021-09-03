#include<iostream>
#include<bitset>
#include<vector>
#include<fstream>

using namespace std;

bitset<32> SHR(bitset<32> x, int n);
bitset<32> ROTR(bitset<32> x, int n);
bitset<32> XOR(bitset<32> x, bitset<32> y);
bitset<32> ADD(bitset<32> x, bitset<32> y);
bitset<32> sigma0(bitset<32> x);
bitset<32> sigma1(bitset<32> x);
bitset<32> theta0(bitset<32> x);
bitset<32> theta1(bitset<32> x);
bitset<32> choice(bitset<32> x, bitset<32> y, bitset<32> z);
bitset<32> majority(bitset<32> x, bitset<32> y, bitset<32> z);


int main(){
	vector< bitset<32> > constant; //Constants - cube roots of the first 64 prime numbers, in binary
	cout<<"Initializing Program......";
	
	return 0;
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
    bool carry = 0;
    bool sum;
    bitset<32> ans;
    for (int i = 0; i<=32; i++){
        sum = (x[i]^y[i])^carry; //Fulladder logic for SUM
        carry = (((x[i]^y[i]) & carry) | (x[i] & y[i])); //Fulladder logic for CARRY
        ans[i] = sum;
    }
    return ans;
}

bitset<32> sigma0(bitset<32> x){
	return XOR(XOR(ROTR(x, 7), ROTR(x, 18)), SHR(x, 3));
}

bitset<32> sigma1(bitset<32> x){
	return XOR(XOR(ROTR(x, 17), ROTR(x, 19)), SHR(x, 3));
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
