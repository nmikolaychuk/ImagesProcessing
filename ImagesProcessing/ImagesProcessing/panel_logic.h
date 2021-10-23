#pragma once


/**
����������� ����� �����������.
������� ��������� ����:
x - ���������� x �������;
y - ���������� y �������;
value - �������� ������� (�������, double);
color_value - �������� ������� (��������, ����).
*/
class Pixel {
public:
	double x;
	double y;
	double value;
	int color_value = 0;

	// ����������� �� ���������.
	Pixel() {
		x = 0.0;
		y = 0.0;
		value = 0.0;
	}

	/**
	����������� � ����� �����������.

	@param in_x: ���������� "x" �������.
	@param in_y: ���������� "y" �������.
	@param in_value: �������� �������.
	*/
	Pixel(double in_x, double in_y, double in_value) {
		x = in_x;
		y = in_y;
		value = in_value;
	}

	/**
	������� �������� ������� � ����.

	@param value: �������� �������.
	@param min_value: ����������� �������� ������� � �����������.
	@param max_value: ������������ �������� ������� � �����������.
	@return ���� ������� [0 - 255].
	*/
	int get_color_from_value(double value, double min_value, double max_value) {
		// �������� �������� ������� �� ������ ����.
		double color_range = (max_value - min_value) / 256.;
		
		for (int i = 0; i <= 255; i++) {
			double limit_value = color_range * i + color_range;
			if (value <= limit_value) return i;
		}
	}
};