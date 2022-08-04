
// https://www.youtube.com/watch?v=6OoSgY6NVVk

#include<iostream>
#include<chrono>
#include<array>
#include<vector>
#include<list>
#include<deque>
#include<set>
#include<unordered_set>
#include<map>
#include<unordered_map>

int main()
{
	std::map<std::string, int> container_map;
	std::cout << "Ordered map" << std::endl;

	container_map["one"] = 1;
	container_map["two"] = 2;
	container_map["three"] = 3;
	container_map["four"] = 4;
	container_map["five"] = 5;
	container_map["six"] = 6;

	for (const auto& ii : container_map)
	{
		std::cout << ii.first << " = " << ii.second << "\n";
	}

	std::unordered_map<std::string, int> container_umap;
	std::cout << "\nUnordered map" << std::endl;

	container_umap["one"] = 1;
	container_umap["two"] = 2;
	container_umap["three"] = 3;
	container_umap["four"] = 4;
	container_umap["five"] = 5;
	container_umap["six"] = 6;

	for (const auto& ii : container_umap)
	{
		std::cout << ii.first << " = " << ii.second << "\n";
	}

	srand(time(0));

	// Lambda function to roll a die
	auto roll = []() { return rand() % 6 + 1; };

	// Create container
	std::vector<int> container;
	std::cout << "\nVector" << std::endl;

	//std::list<int> container;
	//std::cout << "List" << std::endl;

	//std::deque<int> container;
	//std::cout << "Deque" << std::endl;

	//std::set<int> container;
	//std::cout << "Set" << std::endl;

	//std::unordered_set<int> container;
	//std::cout << "Unordered set" << std::endl;

	// Add one item to the container (by default)
	container.push_back(roll());

	// container.insert(roll());	// set container

	// Grab the pointer of where the first element of the container is stored 
	// when the container is created and a first item is added to it
	const int* address_of_original_item_zero_ptr = &(*container.begin());

	// Stores time it takes to add an item to a container
	std::chrono::duration<double> insert_time_duration(0);

	// Each time a user presses an enter key in the command prompt another dice roll is added to the container and 
	// display container's content and various aspects of the memory associated with that container
	do
	{
		// Get address of the first item
		const int* address_of_item_zero_ptr = &(*container.begin());

		std::cout << "Contains " << container.size() << " elements, took " 
			<< std::chrono::duration_cast<std::chrono::microseconds>(insert_time_duration).count() << "us\n";

		for (const auto& i : container)
		{
			const int* address_of_item_x_ptr = &i;
			long long item_offset = address_of_item_x_ptr - address_of_item_zero_ptr;
			long long item_offset_original = address_of_item_x_ptr - address_of_original_item_zero_ptr;

			std::cout << "\tOffset from original:  " << item_offset_original 
				<< "\tOffset from zero: " << item_offset << "\tContent: " << i << "\n";
		}

		auto timestamp1 = std::chrono::high_resolution_clock::now();
		container.push_back(roll());
		//container.insert(roll());	// set container
		auto timestamp2 = std::chrono::high_resolution_clock::now();
		insert_time_duration = (timestamp2 - timestamp1);

	} while (getc(stdin));
}