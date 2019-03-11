#include "GridRow.h"

//Prints an integer vector. useful for debugging
void print_vector(std::vector<int> lst, int offset) {
	string vec = "{";
	for (int i = offset; i < lst.size()+offset; i++) {
		int index = i % lst.size();
		vec.append(std::to_string(lst[index]));
		if (i != lst.size()+offset-1)
			vec.append(", ");
	}
	vec.append("}");
	CULog(vec.c_str());
}

void GridRow::reset() {
	offset = 0;
}

void GridRow::translate(int reference_offset) {
	int n = data.size();
	offset += _relative_speed * reference_offset;
	offset = ((offset % n) + n) % n;
	print_vector(data, offset);
}


void GridRow::setRelativeSpeed(float relative_speed) {
	_relative_speed = relative_speed;
}

int GridRow::getBlock(int index) {
	int n = data.size();
	int i = ((offset+index % n) + n) % n;
	return data[i];
}

void GridRow::setBlock(int index, int value) {
	data[index] = value;
}

std::shared_ptr<GridRow> GridRow::alloc(int row_index, int row_width) {
	shared_ptr<GridRow> gridrow = make_shared<GridRow>();
	gridrow->data = {};
	gridrow->data.resize(row_width);
	gridrow->_width = row_width;
	gridrow->offset = 0;
	gridrow->_relative_speed = 1;
	return gridrow;
}