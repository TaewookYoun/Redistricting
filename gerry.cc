#include<iostream>
#include<vector>
#include<string>
#include<random>

using std::string;
using std::vector;
using std::endl;
using std::cout;
using std::cin;

long int combination(int n, int k){
	if (k == n or k == 0)
		return 1;
	else if (k > n)
		return 0;
	else
		return (combination(n-1, k-1) + combination(n-1, k));
}
// the number of k-tuples of integers whose sum is n
vector<vector<long int>> partition_function(int n, int k){
	long int multiset = combination(n+k-1, k-1);
	// multiset times k
	vector<vector<long int>> temp;
	vector<long int> rows(k);
	if (k == 1)
		temp.at(0).at(0) = n;
	else if(k == 2)
		for (int i = 1 ; i < n ; i++){
				rows.at(0) = i;
				rows.at(1) = n - i;
				temp.push_back(rows);
		}
	else{
		for (int i = 1 ; i <= (n - k + 1) ; i++){
			vector<vector<long int>> another_temp;
			another_temp = partition_function(n-i, k-1);
			for (auto e : another_temp){
				e.push_back(i);
				temp.push_back(e);
			};
		}
	}
	return temp;

}


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
		int get_ID() { return voter_ID; }
		int get_aff() { return affiliation; }
};

class District{
	private:
		vector<Voter> district;
	public:
		District(Voter input){
			district.push_back(input);
		}
		District(vector<Voter> input){
			for ( auto e : input)
			district.push_back(e);
		}
		District(string input){
			for ( int i = 0; i < input.size() ; i++){
				if( input[i] == '+'){
					Voter temp(i,+1);
					district.push_back(temp);}
				else if( input[i] == '-'){
					Voter temp(i,-1);
					district.push_back(temp);}
				else 
					cout << "Error in creating District " << input << endl;
			}
		}
		int size(){ return district.size();}
		int majority(){
			int sum = 0;
			for (auto e : district)
				sum += e.get_aff();
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
		void push_back(Voter input) { district.push_back(input); }
		District sub(int first, int last){
			string temp;
			for (int i = first - 1 ; i < last ; i++){
				if (district.at(i).get_aff() == 1)
					temp += "+";
				else if (district.at(i).get_aff() == -1)
					temp += "-";
				else
					temp += "?";
			}
			District subset(temp);
			return subset;
		}
		Voter at(int i) { return district.at(i); }
		string print(){
			string temp;
			for (auto e : district){
				if ( e.get_aff() == 1)
					temp += "+";
				else if ( e.get_aff() == -1)
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
		Districting(vector<District> districting): districting(districting) {};
		Districting extend_with_new_district(Voter input){
			vector<District> temp_districting = districting;
			District temp_District(input);
			temp_districting.push_back(temp_District);
			Districting temp(temp_districting);
			return temp;
		}
		Districting extend_last_district(Voter input){
			vector<District> temp_districting = districting;
			temp_districting.at(districting.size()-1).push_back(input);
			Districting temp(temp_districting);
			return temp;
		}
		int size() { return districting.size(); }
		int districting_majority() {
			int sum = 0;
			for (auto e : districting){
				sum += e.lean();
			}
			return sum;
		}
		int rule(){
			if (districting_majority() > 0)
				return 1;
			else if (districting_majority() < 0)
				return -1;
			else 
				return 0;
		}
		void print(){
			cout << "[";
			for (auto e : districting){
				cout << "[";
				for (int i = 0; i < e.size() ; i++)
					cout << e.at(i).get_ID() << ":" <<  e.at(i).get_aff() << ",";
				cout << "]";
			}
			cout << "]" << endl;
		}
};


class Population{
	private:
		vector<Voter> people;
	public:
		Population(Voter input){
			people.push_back(input);
		}
		Population(vector<Voter> input){
			for ( auto e : input)
			people.push_back(e);
		}
		Population(string input){
			for ( int i = 0; i < input.size() ; i++){
				if( input[i] == '+'){
					Voter temp(i,+1);
					people.push_back(temp);}
				else if( input[i] == '-'){
					Voter temp(i,-1);
					people.push_back(temp);}
				else 
					cout << "Error in creating District " << input << endl;
			}
		}
		Population(int population_size, int majority, bool trace=false){
			if ( (population_size + majority)%2 != 0) {
				cout << "invalid majority: majority will be reduced by 1" << endl;
				majority -= 1;}
			int maj, min;
			maj = (population_size + majority) / 2;
			min = (population_size - majority) / 2;
			for (int i = 0; i < population_size; i++){
				if (maj == 0){
					Voter temp(i,-1);
					people.push_back(temp);}		
				else if (min == 0){
					Voter temp(i,+1);
					people.push_back(temp);}
				else{
					std::mt19937 rng;
					rng.seed(std::random_device()());
					std::uniform_int_distribution<std::mt19937::result_type> dist(1,population_size);
					int seed = dist(rng);
					if ( seed > min ){
						Voter temp(i,+1);
						people.push_back(temp);
						maj--;}
					else if ( seed <= min){
						Voter temp(i,-1);
						people.push_back(temp);
						min--;}
				}
			}
			if (trace == true)
				print();
		}
		int size(){ return people.size();}
		int majority(){
			int sum = 0;
			for (auto e : people)
				sum += e.get_aff();
			return sum;
		}
		int rule(){
			if (majority() > 0)
				return 1;
			else if (majority() < 0)
				return -1;
			else 
				return 0;
		}
		Voter at(int i) { return people.at(i); }
		Population sub(int first, int last){
			string temp;
			for (int i = first - 1 ; i < last ; i++){
				if (people.at(i).get_aff() == 1)
					temp += "+";
				else if (people.at(i).get_aff() == -1)
					temp += "-";
				else
					temp += "?";
			}
			Population subset(temp);
			return subset;
		}
		void print(){
			cout << "[";
			for (auto e : people)
				cout << e.get_ID() << ":" <<  e.get_aff() << ",";
			cout << "]" << endl;
		}
		Districting splitting(vector<long int> division){
			Districting temp;
			int voters = 0;
			for (auto e : division){
				for (int i = 0; i < e; i++){
					if ( i == 0)
						temp = temp.extend_with_new_district(people.at(voters));
					else
						temp = temp.extend_last_district(people.at(voters));
					voters++;
							}
			}
			return temp;
		}
		Districting minority_rules(int ndistricts){
			Districting temp, another_temp;
			int temp_lean = 0;
			auto partition = partition_function(people.size(), ndistricts);
			for (auto e : partition){
				another_temp = splitting(e);
				if (majority() > 0){
					if (another_temp.rule() <= temp_lean)
						temp = another_temp;}
				else {
					if (another_temp.rule() >= temp_lean)
						temp = another_temp;}
				}
			
			return temp;
		}
};

int main()
{

	Population five("+++--");
	cout << "Redistricting population: " << endl;
	five.print();
	cout << ".. majority rule: " << five.rule() << endl;
	int ndistricts{3};
	auto gerry = five.minority_rules(ndistricts);
	gerry.print();
	cout << ".. minority rule: " << gerry.rule() << endl;

	//testing partitioning
/*	int k = 4;
	auto partv = partition_function(6,k);
	for (auto e : partv){
		cout << "(" ;
		for (int i = 0 ; i < k; i++)
			cout << e.at(i) << ",";
		cout << ")" << endl;};
*/
	return 0;

}




