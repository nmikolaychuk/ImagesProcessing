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
	double mod;

	void get_module_value();
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
	Complex value;
	int color_value;

	/**
	����������� �� ���������.
	*/
	Pixel();

	/**
	����������� � ����� �����������.

	@param in_value: �������� �������.
	*/
	Pixel(double in_value);

	/**
	������� �������� ������� � ����.

	@param min_value: ����������� �������� ������� � �����������.
	@param max_value: ������������ �������� ������� � �����������.
	@return �������� ����� [0-255].
	*/
	int get_color_from_value(double min_value, double max_value, bool is_module);

	/**
	���������� ������ ������������ �����.

	@return None
	*/
	void calc_module_of_cmplx();
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
	bool circle_window;
	bool rect_window;
	int filtration_percent;
	double step_x;
	double step_y;

	/**
	����������� �� ���������.
	*/
	DrawSettings();

	/**
	����������� � �������������� ����������.
	*/
	DrawSettings(CWnd* pic_wnd, CDC* pic_dc, CRect pic, double n_xmax, double n_ymax,
				 bool is_crcl_wnd, bool is_rct_wnd, int filtration_percent);

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
	bool _is_zeros_addition;

public:
	int image_width;
	int image_height;

	int window_offset_x;
	int window_offset_y;
	int circle_radius;

	vector<vector<Pixel>> input_image;
	vector<vector<Pixel>> noise_image;
	vector<vector<Pixel>> spectrum_image_calc;
	vector<vector<Pixel>> spectrum_image_vis;
	vector<vector<vector<Pixel>>> inverted_spectrum_quanters;
	vector<vector<Pixel>> filtered_image;
	DrawSettings ds_input;
	DrawSettings ds_noise;
	DrawSettings ds_spec;
	DrawSettings ds_filtered;

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
	Image(bool is_model, bool _is_zeros_addition, int width, int height, DrawSettings ds_in,
		  DrawSettings ds_noi, DrawSettings ds_sp, DrawSettings ds_ft);

	/**
	���������� ��������� (����������) �������.

	@param is_model: ������������� ������ � �������� ��������� �����������, ���� True.
		��������� ������������ ����������� � �������� ���������, ���� False.
	@param ds: ��������� ���������.
	@param width: ������ �����������.
	@param height: ������ �����������.
	@return None
	*/
	void update_image_condition(bool is_model, bool is_zeros_add, int width, int height);

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

	@return None
	*/
	void get_spectrum_image();

	/**
	���������� �����������.

	@return None
	*/
	void get_filtered_image();

	/**
	�������� ������������ �������.
	*/
	vector<Pixel> vector_interpolation(vector<Pixel> vec, int new_size, bool is_width);

	/**
	������������� ��������� ����������� �� ������� ������� ������.

	@return None
	*/
	void get_addition_image();

	/**
	��������� �����������.
	
	@return None
	*/
	void draw_image(vector<vector<Pixel>> image, DrawSettings ds, bool draw_window);

	/**
	���������� ����� �� ��������� �����������.

	@return ����������� � ������������ �������.
	*/
	vector<vector<Pixel>> calculate_color_of_image(vector<vector<Pixel>> image_vec, bool is_module);

	/**
	������� ���������� �����������.

	@param image_vec: ������ �������� ���������� ����������� ��� �������.
	@return ��������� ������.
	*/
	vector<vector<Pixel>> clear_selected_image(vector<vector<Pixel>> image_vec);

	/**
	���������� ������� ��� ��������� � ���������������� �������.

	@return �������.
	*/
	double calculate_difference();

	/**
	���������� ������� ������ ����� � ���������� �����������.
	
	@param data: �����������.
	@return None
	*/
	void calculate_modules(vector<vector<Pixel>>& data);
};