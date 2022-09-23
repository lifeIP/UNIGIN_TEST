// THIS FILE IS STUB FOR INPUT AND OUTPUT DATA
// DO NOT MODIFICATE IT - IT`S PART OF ANOTHER PROJECT

// это файл-заглушка, для демонстрации как метод будет вызван.
// не модифицируйте его - возможно это часть другого проекта

#include <iostream>
#include <fstream>

#include "task.h"

int main()
{
	std::vector<Triangle> triangles;
	std::vector<int> intersections;
	// input
	int count = 0;

	std::ifstream in("C:\\Users\\molok\\Desktop\\UNIGIN\\LOGIC\\public\\bin\\input.txt");
	if (in.is_open())
	{
		in >> count;
		Triangle t;
		for (int i = 0; i < count; i++)
		{
			in >> t.a.x >> t.a.y;
			in >> t.b.x >> t.b.y;
			in >> t.c.x >> t.c.y;
			triangles.push_back(t);
		}
		in.close();
	}

	// invoke
	Task::checkIntersections(triangles, intersections);

	if (intersections.empty())
	{
		return -1;
	}

	// output
	std::ofstream out;
	out.open("output.txt");
	if (out.is_open())
	{
		for (int i = 0; i < count; i++)
			out << i << " " << intersections[i] << std::endl;
		out.close();
	}

	return 0;
}
