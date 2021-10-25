#pragma once
#include <math.h>
#include <algorithm>
#include <vector>
using namespace std;


/**
���������� ������������ �����.

*/
class Complex {
public:
	double re;
	double im;
};


/**
����������� ����� �����������.

������� ��������� ����:
x - ���������� x �������;
y - ���������� y �������;
value - �������� ������� (�������, Complex);
color_value - �������� ������� (��������, ����).
*/
class Pixel {
public:
	double x;
	double y;
	Complex value;
	int color_value;

	/**
	����������� �� ���������.
	*/
	Pixel();

	/**
	����������� � ����� �����������.

	@param in_x: ���������� "x" �������.
	@param in_y: ���������� "y" �������.
	@param in_value: �������� �������.
	*/
	Pixel(double in_x, double in_y, double in_value);

	/**
	������� �������� ������� � ����.

	@param min_value: ����������� �������� ������� � �����������.
	@param max_value: ������������ �������� ������� � �����������.
	@return �������� ����� [0-255].
	*/
	int get_color_from_value(double min_value, double max_value);

	/**
	���������� ������ ������������ �����.

	@return ������ �����.
	*/
	double calc_module_of_cmplx();
};


/**
��������� ���������.
*/
class DrawSettings {
public:
	// ��������� ����.
	CWnd* _pic_wnd;
	CDC* _pic_dc;
	CRect _pic;
	// ������� �����������.
	double width, height;
	// �������� ���������.
	double xmin, xmax, ymin, ymax;
	double xp, yp;

	/**
	����������� �� ���������.
	*/
	DrawSettings();

	/**
	����������� � �������������� ����������.
	*/
	DrawSettings(CWnd* pic_wnd, CDC* pic_dc, CRect pic, double n_xmax, double n_ymax);

	/**
	������� ������� ��������� � ��������.
	*/
	pair<double, double> get_screen_coordinates(double world_x, double world_y);
};


/**
���������� ��������� �����������.
*/
class Image {
private:
	bool _is_model_image;

public:
	int image_width;
	int image_height;

	vector<Pixel> input_image;
	vector<Pixel> noise_image;
	vector<Pixel> spectrum_image;
	vector<Pixel> filtered_image;
	DrawSettings ds_input;
	DrawSettings ds_noise;

	// ����������� �� ���������.
	Image();

	/**
	����������� � �����������.

	@param is_model: ������������� ������ � �������� ��������� �����������, ���� True.
		��������� ������������ ����������� � �������� ���������, ���� False.
	@param width: ������ �����������.
	@param height: ������ �����������.
	@param ds_input: ��������� ��������� ��� ��������� �����������.
	@param ds_noise: ��������� ��������� ��� ������������ �����������.
	@return None
	*/
	Image(bool is_model, int width, int height, DrawSettings ds_in, DrawSettings ds_noi);

	/**
	���������� ��������� (����������) �������.

	@param is_model: ������������� ������ � �������� ��������� �����������, ���� True.
		��������� ������������ ����������� � �������� ���������, ���� False.
	@param ds: ��������� ���������.
	@param width: ������ �����������.
	@param height: ������ �����������.
	@return None
	*/
	void update_image_condition(bool is_model, int width, int height);

	/**
	��������� ����������� � ���� ����� ��������� �������.

	@return None
	*/
	void get_input_image();

	/**
	��������� ���� �� �������� �����������.

	@param noise_percent: ������� ���� %.
	@return None
	*/
	void get_noise_image(int noise_percent);

	/**
	���������� ������� �����������.

	@return ������ �����������.
	*/
	vector<Pixel> get_spectrum_image();

	/**
	���������� �����������.

	@return ��������������� �����������.
	*/
	vector<Pixel> get_filtered_image();

	/**
	������������� ��������� ����������� �� ������� ������� ������.

	@return �������������� �����������.
	*/
	vector<Pixel> get_addition_input_image();

	/**
	��������� �����������.
	
	@return None
	*/
	void draw_image(vector<Pixel> image, DrawSettings ds);

	/**
	���������� ����� �� ��������� �����������.

	@return ����������� � ������������ �������.
	*/
	vector<Pixel> calculate_color_of_image(vector<Pixel> image_vec);

	/**
	������� ���������� �����������.

	@param image_vec: ������ �������� ���������� ����������� ��� �������.
	@return ��������� ������.
	*/
	vector<Pixel> clear_selected_image(vector<Pixel> image_vec);
};