#include <iostream>
#include <vector>
#include <cmath>
#include  <cstdint>

#include <sstream>

class bloom_filter {
	int m; //size of structure
	float p;  //probability
	const int M = 2147483647;
	std::vector <bool> data;
	int hash_count; 

	std::vector <int> prime_numbers={2};



	int hash(const uint64_t& x, const int& i) {
		auto m1 = x % M;
		auto m2 = (i + 1) % M;
		return ((m1 * m2 + prime_numbers[i]) % M) % m;
	}

	bool is_prime(int n) {
		for (int i = 0; i < prime_numbers.size(); i++)
			if (n % prime_numbers[i] == 0)
				return false;
		return true;
	}

public:
	bloom_filter() = default;

	int get_hash_count() {
		return hash_count;
	}

	int get_m() {
		return m;
	}

	bool init(int n, float P) {
		p = P;
		m = static_cast<int>(-n * std::log2(P) / std::log(2) + 0.5);
		hash_count = static_cast<int>(-std::log2(P) + 0.5);
		data.resize(m);
		if (hash_count == 0) {
			return false;
		}
		int temp = 3;
		while (hash_count != prime_numbers.size()) {
			if (is_prime(temp)) {
				prime_numbers.push_back(temp);
			}
			++temp;
		}
		
		return true;
	}

	void add(const uint64_t& K) {
		for (size_t i = 0; i < hash_count; i++) {
			data[hash(K, i)] = true;
		}
	}

	bool search(const uint64_t& K) {
		for (size_t i = 0; i < hash_count; i++) {
			if (data[hash(K, i)] == false) {
				return false;
			}
		}
		return true;
	}

	friend void print(const bloom_filter& b, std::stringstream& stream);
};



void print(const bloom_filter& b, std::stringstream& stream) {

	for (auto el : b.data)
	{
		if (el) {
			stream << '1';
		}
		else {
			stream << '0';
		}
	}
	stream << std::endl;
}


void split(std::string& line, std::vector<std::string>& split_line) {
	split_line.clear();
	std::string temp = "";
	for (size_t i = 0; i < line.length(); i++) {
		if (line[i] != ' ') {
			temp += line[i];
		}
		else {
			split_line.push_back(temp);
			temp.clear();
		}
		if (i == line.length() - 1) {
			split_line.push_back(temp);
		}
	}
}

int main() {
	std::ios_base::sync_with_stdio(0);
	std::stringstream stream;



	bloom_filter b;
	std::string str;
	std::vector<std::string> split_str;
	while (std::getline(std::cin, str)) {
		split(str, split_str);
		if (str.empty()) {
			continue;
		}
		else if (split_str[0] == "set" && split_str.size() == 3) {
			float p;
			int  n;
			try {
				p = std::stof(split_str[2]);
				n = std::stoull(split_str[1]);
			}
			catch (std::exception) {
				stream << "error" << std::endl;
				continue;
			}
			if (p > 1 || n == 0 || p<0) {
				stream << "error" << std::endl;
				continue;
			}

			if (b.init(n, p)) {
				stream << b.get_m() << " " << b.get_hash_count() << "\n";
				break;
			}
			else {
				stream << "error" << std::endl;
				continue;
			}
		}
		else {
			stream << "error" << std::endl;
		}
	}

	while (std::getline(std::cin, str)) {
		split(str, split_str);
		if (str.empty()) {
			continue;
		}
		else if (split_str[0] == "add" && split_str.size() == 2) {
			try {
				uint64_t temp = std::stoull(split_str[1]);
				b.add(temp);
			}
			catch (std::exception) {
				stream << "error" << std::endl;
				continue;
			}

		}

		else if (split_str[0] == "search" && split_str.size() == 2) {
			try {
				uint64_t temp = std::stoull(split_str[1]);
				if (b.search(temp)) {
					stream << '1' << std::endl;
				}
				else {
					stream << '0' << std::endl;
				}
			}
			catch (std::exception) {
				stream << "error" << std::endl;
				continue;
			}

		}
		else if (split_str[0] == "print" && split_str.size() == 1) {
			print(b, stream);
		}
		else {
			stream << "error" << std::endl;
		}
	}

	std::cout << stream.str();

}