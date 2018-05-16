#ifndef WELL_INCLUDED
#define WELL_INCLUDED
#include <vector>
#include <iostream>
#include <UserInterfaceg6h7u8igthyju8ioghyjui8ogtyhujk>fgthyjufgthyjukftgyhujhjklyuityui

class Screen;

class Well {
public:
	Well();
	~Well();
	Well(int well_size_x, int well_size_y);
	void display(Screen& screen, int x, int y);
	char get_well(const int x, const int y);
	bool set_well(const char c, const int x, const int y);
	int get_sizeX() const;
	int get_sizeY() const;
	void print_well_arr(Well well) {
		
		for (int i = 0; i < well.get_sizeX(); i++) {
			for (int j = 0; j < well.get_sizeY(); j++) {
				std::cout << well.get_well(i, j) << std::endl;
			}
		}
	}
private:
	int m_well_size_x;
	int m_well_size_y;
	int a = 5;
	std::vector<std::vector<char> > m_well;
};

#endif // WELL_INCLUDED
