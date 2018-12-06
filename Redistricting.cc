#include<iostream>
#include<vector>
#include<string>
#include<random>

using std::string;
using std::vector;
using std::endl;
using std::cout;
using std::cin;

class Voter{
	private:
		int voter_ID;
		int affiliation;
	public:
		Voter (int voter_ID, int affiliation): voter_ID(voter_ID), affiliation(affiliation) {}
		void print() {
			switch (affiliation){
				case 1 : 
					cout << voter_ID << ":+" << endl;
					break;
				case -1 :
					cout << voter_ID << ":-" << endl;
					break;
				case 0 :
					cout << voter_ID << ":0" << endl;
					break;
				default :
					cout << "Illegal affiliation value: " << affiliation << endl;
					cout << "Error in creating voter " << voter_ID << endl;
					break;}
		}
		int get_aff() { return affiliation; }
};

class District{
	private:
		vector<int> district;
	public:
		District(Voter input){
			district.push_back(input.get_aff());
		}
		District(vector<Voter> input){
			for ( auto e : input)
			district.push_back(e.get_aff());
		}
		District(string input){
			for ( auto e : input){
				if( e == '+')
					district.push_back(1);
				else if( e == '-')
					district.push_back(-1);
				else 
					cout << "Error in creating District " << input << endl;
			}}
		int size(){ return district.size();}
		int majority(){
			int sum = 0;
			for (auto e : district)
				sum += e;
			return sum;
		}
		int lean(){
			if (majority() > 0)
				return 1;
			else if (majority() < 0)
				return -1;
			else 
				return 0;
		}
		District sub(int first, int last){
			string temp;
			for (int i = first - 1 ; i < last ; i++){
				if (district.at(i) == 1)
					temp += "+";
				else if (district.at(i) == -1)
					temp += "-";
				else
					temp += "?";
			}
			District subset(temp);
			return subset;
		}
		string print(){
			string temp;
			for (auto e : district){
				if ( e == 1)
					temp += "+";
				else if ( e == -1)
					temp += "-";
				else
					temp += "?";
			}
			return temp;
		}
};

class Population{
	private:
		vector<int> people;
	public:
		Population(Voter input){
			people.push_back(input.get_aff());
		}
		Population(vector<Voter> input){
			for ( auto e : input)
			people.push_back(e.get_aff());
		}
		Population(string input){
			for ( auto e : input){
				if( e == '+')
					people.push_back(1);
				else if( e == '-')
					people.push_back(-1);
				else 
					cout << "Error in creating Population " << input << endl;
			}
		}
		Population(int population_size, int majority, bool trace=false){
			people = vector<int>(population_size);
			if ( (population_size + majority)%2 != 0) {
				cout << "invalid majority: majority will be reduced by 1" << endl;
				majority -= 1;}
			int maj, min;
			maj = (population_size + majority) / 2;
			min = (population_size - majority) / 2;
			for (int i = 0; i < population_size; i++){
				if (maj == 0)
					people.at(i) = -1;		
				else if (min == 0)
					people.at(i) = +1;
				else{
					std::mt19937 rng;
					rng.seed(std::random_device()());
					std::uniform_int_distribution<std::mt19937::result_type> dist(1,population_size);
					int seed = dist(rng);
					if ( seed > min ){		
						people.at(i) = +1;
						maj--;}
					else if ( seed <= min){
						people.at(i) = -1;
						min--;}
				}
			}
			if (trace == true)
				cout << print() << endl;
		}
		int size(){ return people.size();}
		int majority(){
			int sum = 0;
			for (auto e : people)
				sum += e;
			return sum;
		}
		int lean(){
			if (majority() > 0)
				return 1;
			else if (majority() < 0)
				return -1;
			else 
				return 0;
		}
		Population sub(int first, int last){
			string temp;
			for (int i = first - 1 ; i < last ; i++){
				if (people.at(i) == 1)
					temp += "+";
				else if (people.at(i) == -1)
					temp += "-";
				else
					temp += "?";
			}
			Population subset(temp);
			return subset;
		}
		string print(){
			string temp;
			for (auto e : people){
				if ( e == +1)
					temp += "+";
				else if ( e == -1)
					temp += "-";
				else
					temp += "?";
			}
			return temp;
		}
};

class Districting{
	private:
		vector<District> districting;
	public:
		Districting() {};
		int size() { return districting.size(); }
		int majority() {
			int sum = 0;
			for (auto e : districting){
				sum += e.majority();
			}
			return sum;
		}
		int lean(){
			if (majority() > 0)
				return 1;
			else if (majority() < 0)
				return -1;
			else 
				return 0;
		}
};

int main()
{	
	cout << "Making single voter population B" << endl;
	Population people( vector<Voter>{ Voter(0,+1) } );
	cout << ".. size: " << people.size() << endl;
	cout << ".. lean: " << people.lean() << endl;
	Districting gerry;
	cout << "Start with empty districting:" << endl;
	cout << ".. number of districts: " << gerry.size() << endl;
	
	return 0;

}




