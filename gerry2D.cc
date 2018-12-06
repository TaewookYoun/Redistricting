#include<iostream>
#include<vector>
#include<string>
#include<cmath>
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
vector<vector<int>> partition_function(int n, int k){
	long int multiset = combination(n+k-1, k-1);
	// multiset times k
	vector<vector<int>> temp;
	vector<int> rows(k);
	if (k == 1)
		temp.at(0).at(0) = n;
	else if(k == 2)
		for (int i = 1 ; i < n ; i++){
				rows.at(0) = i;
				rows.at(1) = n - i;
				temp.push_back(rows);
		}
	else{
		for (int i = 1 ; i <= (n-k+1) ; i++){
			vector<vector<int>> another_temp;
			another_temp = partition_function(n-i, k-1);
			for (auto e : another_temp){
				e.push_back(i);
				temp.push_back(e);
			};
		}
	}
	return temp;
}

class District_2D{
	private: 
		vector<vector<int>> district_2D;
		int rows, cols;
	public:
		// m by n matrix initially with all being -1
		District_2D(int m, int n){
			district_2D = vector<vector<int>>(m,vector<int>(n,-1));
			rows = m;
			cols = n;
		}
		int get_rows() { return rows; }
		int get_cols() { return cols; }
		vector<vector<int>> get_2D() { return district_2D; }
		//finding starting point of the 2D partition function
		vector<int> starting_point(){
			vector<int> temp(2);
			int i, j;
			for (i = 0 ; i < rows ; i++){
				for (j = 0 ; j < cols ; j++){
					if (district_2D.at(i).at(j) == -1){
						temp.at(0) = i;
						temp.at(1) = j;
						goto escape;
					}
				}
			}
escape:
			return temp;
		}
		void print(){
			for (int i = 0 ; i < rows ; i++){
				for (int j = 0 ; j < cols ; j++){
					cout << "[" <<district_2D.at(i).at(j) << "]";
				}
				cout << endl;
			}
		}
		void plugin(vector<int> coordinates, int k){
			district_2D.at(coordinates.at(0)).at(coordinates.at(1)) = k;
		}
		void covering(int district_order, vector<char> steps){
			vector<int> coordinates = starting_point();
			plugin(coordinates, district_order);
			for (auto e : steps){
				switch (e){
					case 'u':
						if (coordinates.at(0) > 0){
							if (district_2D.at(coordinates.at(0)-1).at(coordinates.at(1)) == -1){
								coordinates.at(0)--;
								plugin(coordinates, district_order);}
						}
						break;
					case 'd':
						if (coordinates.at(0) < rows-1){
							if (district_2D.at(coordinates.at(0)+1).at(coordinates.at(1)) == -1){
								coordinates.at(0)++;	
								plugin(coordinates, district_order);}
						}
						break;
					case 'r':
						if (coordinates.at(1) < cols-1){
							if (district_2D.at(coordinates.at(0)).at(coordinates.at(1)+1) == -1){
								coordinates.at(1)++;
								plugin(coordinates, district_order);}
						}
						break;	
					case 'l':
						if (coordinates.at(1) > 0){
							if (district_2D.at(coordinates.at(0)).at(coordinates.at(1)-1) == -1){
								coordinates.at(1)--;
								plugin(coordinates, district_order);}
						}
						break;
					default :
						break;
				}
			}

		}
};

class Districting_2D{
	private:
		vector<District_2D> districting_2D;
		int rows, cols, elements, ndistricts;
	public:
		Districting_2D(){};
		void set_parameter(int m, int n, int k){
			rows = m;
			cols = n;
			elements = rows*cols;
			ndistricts = k;
		}
		void extend_with_district_2D(District_2D input){;
			districting_2D.push_back(input);
		}
		vector<District_2D> get_2D() { return districting_2D; }
		//generating possible steps
		vector<vector<char>> generating_steps(int k){
			string str = "udrl";
			vector<vector<char>> temp;
			if ( k == 0){
				vector<vector<char>> vtemp(1,vector<char>(1,'0'));	
				temp = vtemp;
			}
			else if (k == 1){
				vector<vector<char>> vtemp(4);
				vtemp.at(0).push_back('u');
				vtemp.at(1).push_back('d');
				vtemp.at(2).push_back('r');
				vtemp.at(3).push_back('l');
				temp = vtemp;
			}
			else{
				vector<vector<char>> vtemp;
				vtemp = generating_steps(k-1);
				for (char ch : str){
					for (auto e : vtemp){
						e.push_back(ch);
						temp.push_back(e);
					};
				}
			}
			return temp;
	
		}
/*
		void complete_covering(vector<int> partition){
			vector<vector<vector<char>>> complete_steps;
			//generating complete steps
			for (int i = 0 ; i < ndistricts ; i++){
				auto temp_steps = generating_steps(partition.at(i));
				complete_steps.push_back(temp_steps);
			}		
			vector<Districting_2D> temp(ndistricts);
			// psuedo districting 2D
			for (auto e : complete_steps.at(0)){
				District_2D temp_0(rows,cols);
				temp_0.covering(0, e);
				//check
				if (checker_seg(temp_0,0,e.size()) == true) 
					temp.at(0).extend_with_district_2D(temp_0);
			}
			for (int i = 1 ; i < ndistricts ; i++){
				for (auto e : complete_steps.at(i)){
					auto temp_i = temp.at(i-1);
					auto e_districting = temp_i.get_2D();
					for (auto l : e_districting)
					{
						l.covering(i, e);
						//check
						if (checker_seg(l,i,e.size()) == true)
							temp.at(i).extend_with_district_2D(l);
					}
				}
			}
			//check and push back into real districting 2D
			auto temp_to_sort = temp.at(ndistricts-1).get_2D();
			for (auto e : temp_to_sort){
				if (checker(e) == true)
					extend_with_district_2D(e);
			}
		}
*/
		//optimizing
		void complete_covering(vector<int> partition){
			vector<Districting_2D> temp(ndistricts);
			//constructing districting 2D
			for (int i = 0 ; i < ndistricts ; i++){
				vector<vector<char>> temp_steps = generating_steps(partition.at(i));
				generating_pseudo_districting_2D(temp, temp_steps, i);
			}		
		//check and push back into real districting 2D
			auto temp_to_sort = temp.at(ndistricts-1).get_2D();
			for (auto e : temp_to_sort){
				if (checker(e) == true)
					extend_with_district_2D(e);
			}
		}
		//generating pseudo districting 2D
		void generating_pseudo_districting_2D (vector<Districting_2D> &temp, vector<vector<char>> temp_steps, int i){
			if (i == 0){
				for (auto e : temp_steps){
					District_2D temp_0(rows,cols);
					temp_0.covering(0, e);
					//check
					if (checker_seg(temp_0,0,e.size()) == true) 
						temp.at(0).extend_with_district_2D(temp_0);
				}
			}
			else{
				for (auto e : temp_steps){
					auto temp_i = temp.at(i-1);
					auto e_districting = temp_i.get_2D();
					for (auto l : e_districting)
					{
						l.covering(i, e);
						//check
						if (checker_seg(l,i,e.size()) == true)
							temp.at(i).extend_with_district_2D(l);
					}
				}

			}
		}
		// checking the partial districting is complete
		bool checker_seg(District_2D input, int district_order, int steps){
			bool is_complete;
			int counter = 0;
			auto testee = input.get_2D();
			for (int i = 0 ; i < rows ; i++){
				for (int j = 0 ; j < cols ; j++){
					if (testee.at(i).at(j) == district_order)
						counter++;
				}
			}
			if (counter == steps+1)
				is_complete = true;
			else
				is_complete = false;
			return is_complete;
		}
		//check if the whole  districting is complete
		bool checker(District_2D input){
			bool is_complete = true;
			auto testee = input.get_2D();
			for (int i = 0 ; i < rows ; i++){
				for (int j = 0 ; j < cols ; j++){
					if (testee.at(i).at(j) == -1)
						is_complete = false;
				}
			}
			return is_complete;
		}
		void print(){
			for (auto e : districting_2D){
				e.print();
				cout << endl;
			}
		}
};

class Voter{
	private:
		int voter_ID;
		int affiliation;
		int district_grid=0;
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
		int get_dist() { return district_grid; }
		void set_dist(int grid) { district_grid = grid; }
};
class Population{
	private:
		vector<vector<Voter>> people;
		int rows=0;
		int cols=0;
		int elements;
		int num_district;
	public:
		Population(){}
		Population(vector<Voter> input){
			people.push_back(input);
			rows++;
			cols = input.size();
		}
		Population(vector<vector<Voter>> input){
			people = input;
		}
		void extend_with_string(string input){
			vector<Voter> vtemp;
			for ( int i = 0; i < input.size() ; i++){
				if( input[i] == '+'){
					Voter temp(i,+1);
					vtemp.push_back(temp);}
				else if( input[i] == '-'){
					Voter temp(i,-1);
					vtemp.push_back(temp);}
				else 
					cout << "Error in creating District " << input << endl;
			}
			people.push_back(vtemp);
			rows++;
			cols = input.size();
			elements = rows*cols;
		}
		int get_rows() { return rows; }
		int get_cols() { return cols; }
		int get_elements() { return elements; }
/*
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
*/
		int size(){ return people.size();}
		int majority(){
			int sum = 0;
			for (auto e : people){
				for (int i = 0 ; i < e.size() ; i++)
				sum += e.at(i).get_aff();
			}
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
		Voter at(int i, int j) { return people.at(i).at(j); }
		void print(){
			for (auto e : people){
				for (int i = 0 ; i < e.size() ; i++)
				cout << "["<< e.at(i).get_ID() << ":" <<  e.at(i).get_aff()  << "]";
				cout << endl;
			}
		}
		void print_wo_dist(){
			for (auto e : people){
				for (int i = 0 ; i < e.size() ; i++)
					cout << "[" <<  e.at(i).get_aff() << "]";
				cout << endl;
			}
		}	
		void print_w_dist(){
			for (auto e : people){
				for (int i = 0 ; i < e.size() ; i++)
					cout << "[" <<  e.at(i).get_aff() << ":" << e.at(i).get_dist() << "]";
				cout << endl;
			}
		}
		void splitting(District_2D input){
			int rows = input.get_rows();
			int cols = input.get_cols();
			auto district_2D = input.get_2D();
			for (int i = 0 ; i < rows; i++){
				for (int j = 0 ; j < cols ; j++){
					people.at(i).at(j).set_dist(district_2D.at(i).at(j)); 
				}
			}
		}
		//majority of each district segment
		int district_majority(int district_order){
			int majority = 0;
			for (int i = 0; i < rows ; i++){
				for (int j = 0 ; j < cols ; j++){
					if (people.at(i).at(j).get_dist() == district_order)
						majority += people.at(i).at(j).get_aff(); 
				}
			}
			if (majority > 0)
				return 1;
			else if (majority < 0)
				return -1;
			else 
				return 0;
		}
		//rule of entire district
		int district_rule(){
			int sum = 0;
			for (int n = 0; n < num_district ; n++)
				sum += district_majority(n);
			if (sum > 0)
				return 1;
			else if (sum < 0)
				return -1;
			else 
				return 0;
		}
		//over every possible way
/*
		void minority_rules(int ndistricts){
			int temp_lean = 0;
			vector<vector<int>> partition = partition_function(elements, ndistricts);
			//substract by 1 for our purpose
			for (int i = 0; i < partition.size() ; i++){
				for (int j = 0; j < ndistricts ; j++)
					partition.at(i).at(j)--;
			}
			for (auto e : partition){
				cout << "(" ;
				for (int i = 0 ; i < e.size(); i++)
					cout << e.at(i) << ",";
				cout << ")" << endl;};
			for (auto e : partition){
				Districting_2D temp;
				temp.set_parameter(rows, cols, ndistricts);
				temp.complete_covering(e);
				for (auto l : temp.get_2D()){
					Population ptemp = *this;
					ptemp.splitting(l);
					int lean = ptemp.district_majority(ndistricts);
					if (lean <= temp_lean)
						splitting(l);
				}
			}
		}
*/
		//over the almost same partitions
		void minority_rules(int ndistricts){
			int temp_lean = 0;
			num_district = ndistricts;
			vector<int> partition;
			for (int i = 0; i < ndistricts ; i++)
				partition.push_back(elements/ndistricts);
			//substract by 1 for our purpose
			for (int i = 0; i < partition.size() ; i++)
				partition.at(i)--;
			int reminder = elements%ndistricts;
			for (int i = 0 ; i < reminder ; i++){
				partition.at(i)++;
			}
			Districting_2D temp;
			temp.set_parameter(rows, cols, ndistricts);
			temp.complete_covering(partition);
			for (auto e : temp.get_2D()){
				Population ptemp = *this;
				ptemp.splitting(e);
				int lean = ptemp.district_rule();
				if (rule() >= 0){
					if (lean < temp_lean){
						splitting(e);
						break;
					}
					else if (lean == temp_lean)
						splitting(e);
				}
				else{
					if (lean > temp_lean){
						splitting(e);
						break;
					}
				}
			}
		}

};

int main(){

	int ndistricts;
	Population five;
	five.extend_with_string("--+++");
	five.extend_with_string("--+++");
	five.extend_with_string("--+++");
	five.extend_with_string("--+++");
	five.extend_with_string("--+++");
	cout << "Original population: " << endl;
	five.print_wo_dist();
	cout << ".. size: " << five.get_elements() << endl;
	cout << ".. majority rule: " << five.rule() << endl;

	ndistricts = 5;
	five.minority_rules(ndistricts);
	five.print_w_dist();
	cout << ".. minority rule: " << five.district_rule() << endl;


	Population gerry;
	gerry.extend_with_string("--+");
	gerry.extend_with_string("-++");
	gerry.extend_with_string("++-");
	gerry.extend_with_string("+--");
	gerry.extend_with_string("---");
	cout << "Original population: " << endl;
	gerry.print_wo_dist();
	cout << ".. size: " << gerry.get_elements() << endl;
	cout << ".. majority rule: " << gerry.rule() << endl;

	ndistricts = 3;
	gerry.minority_rules(ndistricts);
	gerry.print_w_dist();
	cout << ".. minority rule: " << gerry.district_rule() << endl;
/*
	Population six;
	six.extend_with_string("---+++");
	six.extend_with_string("---+++");
	six.extend_with_string("---+++");
	six.extend_with_string("---+++");
	six.extend_with_string("---+++");
	six.extend_with_string("---+++");
	cout << "Original population: " << endl;
	six.print_wo_dist();
	cout << ".. size: " << six.get_elements() << endl;
	cout << ".. majority rule: " << six.rule() << endl;

	ndistricts = 6;
	six.minority_rules(ndistricts);
	six.print_w_dist();
	cout << ".. minority rule: " << six.district_rule() << endl;
*/
/*
	District_2D five_five(10,5);
	vector<char> steps0 = {'r','d','r','d','l'};
	vector<char> steps1 = {'d','d','d','r','r'};
	five_five.covering(0,steps0);
	five_five.covering(1,steps1);
	five_five.print();
	cout << "start" << endl;
	Districting_2D steps;
	steps.set_parameter(2,2,2);
	vector<int> part = {1,1};
	steps.complete_covering(part);
	steps.print();
	cout << "done!" << endl;
*/
/*
	auto ar = steps.generating_steps(0);
	for (auto e : ar){
		for (int i = 0 ; i < e.size() ; i++)
			cout << e.at(i) ;
		cout << endl;
	}
*/
/*
	int ndistricts{3};
	auto gerry = five.minority_rules(ndistricts);
	gerry.print();
	cout << ".. minority rule: " << gerry.rule() << endl;
*/
	//testing partitioning
/*
	int k = 5;
	auto partv = partition_function(5,k);
	for (auto e : partv){
		cout << "(" ;
		for (int i = 0 ; i < k; i++)
			cout << e.at(i) << ",";
		cout << ")" << endl;};
*/
	return 0;

}
