#pragma once
#include "pch.h"
#include "panel_logic.h"
#include <fstream>
using namespace std;

/**
���� ��� ���������� ������ ������ ���������.
*/


using namespace std;

// ****************************** Default ******************************

/**
��������� ����������� �� ���������.
*/
int DEFAULT_WIDTH = 600;
int DEFAULT_HEIGHT = 600;

double calculate_psi() {
	double r = 0;
	int count_of_calc = 20;
	for (int i = 0; i < count_of_calc; i++)
	{
		r += ((rand() % 100) / (100. * 1.0) * 2.) - 1.;		// [-1;1]
	}
	return r / count_of_calc;
}

// ****************************** Pixel ******************************

// ����������� �� ���������.
Pixel::Pixel() {
	x = 0.0;
	y = 0.0;
	value.re = 0.0;
	value.im = 0.0;
	color_value = 0;
}

/**
����������� � ����� �����������.

@param in_x: ���������� "x" �������.
@param in_y: ���������� "y" �������.
@param in_value: �������� �������.
*/
Pixel::Pixel(double in_x, double in_y, double in_value) {
	x = in_x;
	y = in_y;
	value.re = in_value;
	value.im = 0.0;
	color_value = 0;
}

/**
������� �������� ������� � ����.

@param min_value: ����������� �������� ������� � �����������.
@param max_value: ������������ �������� ������� � �����������.
@return None.
*/
int Pixel::get_color_from_value(double min_value, double max_value) {
	// �������� �������� ������� �� ������ ����.
	double color_range = (max_value - min_value) / 256.;

	vector<double> help;
	for (int i = 0; i <= 255; i++) {
		double limit_value = color_range * i + color_range;
		if (value.re <= limit_value) return i;
	}
}

/**
���������� ������ ������������ �����.

@return ������ �����.
*/
double Pixel::calc_module_of_cmplx() {
	return sqrt(value.re * value.re + value.im * value.im);
}

// ****************************** DrawSettings ******************************

/**
����������� �� ���������.
*/
DrawSettings::DrawSettings() {
	_pic_wnd = NULL;
	_pic_dc = NULL;
	_pic = NULL;

	xmin = -1.0;
	xmax = 1.0;
	ymin = -1.0;
	ymax = 1.0;
	xp = 0.0;
	yp = 0.0;
	width = DEFAULT_WIDTH;
	height = DEFAULT_HEIGHT;
}

/**
����������� � �������������� ����������.
*/
DrawSettings::DrawSettings(CWnd* pic_wnd, CDC* pic_dc, CRect pic, double n_xmax=DEFAULT_WIDTH, double n_ymax=DEFAULT_HEIGHT) {
	_pic_wnd = pic_wnd;
	_pic_dc = pic_dc;
	_pic = pic;
	
	xmin = -1.0;
	xmax = 1.0;
	ymin = -1.0;
	ymax = 1.0;
	xp = 0.0;
	yp = 0.0;
	width = n_xmax;
	height = n_ymax;
}

/**
������� ������� ��������� � ��������.

@param world_x: �������� x � "�������" �����������.
@papram world_y: �������� y � "�������" �����������.
@return ���� �������� (x, y) � "��������" �����������.
*/
pair<double, double> DrawSettings::get_screen_coordinates(double world_x, double world_y) {
	pair<double, double> screen_pixel;
	screen_pixel.first = xp * (world_x - xmin);
	screen_pixel.second = yp * (world_y - ymax);
	return screen_pixel;
}

// ****************************** Image ******************************

/**
����������� �� ���������.
*/
Image::Image() {
	_is_model_image = true;
	image_width = DEFAULT_WIDTH;
	image_height = DEFAULT_HEIGHT;
	DrawSettings* dset = new DrawSettings();
	ds_input = *dset;
	ds_noise = *dset;
}

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
Image::Image(bool is_model, int width, int height, DrawSettings ds_in, DrawSettings ds_noi) {
	update_image_condition(is_model, width, height);
	ds_input = ds_in;
	ds_noise = ds_noi;
}

/**
	���������� ��������� (����������) �������.

	@param is_model: ������������� ������ � �������� ��������� �����������, ���� True.
		��������� ������������ ����������� � �������� ���������, ���� False.
	@param ds: ��������� ���������.
	@param width: ������ �����������.
	@param height: ������ �����������.
	@return None
	*/
void Image::update_image_condition(bool is_model, int width, int height) {
	_is_model_image = is_model;
	image_height = height;
	image_width = width;
}

/**
��������� ����������� � ���� ����� ��������� �������.

@return None
*/
void Image::get_input_image() {
	// ����� ��������� �������.
	int number_of_gaussian = 3;
	// ��������� �������.
	vector<double> amplitude{ 1, 1.5, 2 };
	vector<double> sigma_x{ ds_input.width * 0.1, ds_input.width * 0.09, ds_input.width * 0.06 };
	vector<double> sigma_y{ ds_input.height * 0.12, ds_input.height * 0.08, ds_input.height * 0.07 };
	vector<double> center_x{ 0.1 * ds_input.width, 0.5 * ds_input.width, 0.8 * ds_input.width };
	vector<double> center_y{ 0.2 * ds_input.height, 0.5 * ds_input.height, 0.75 * ds_input.height };

	// ���������� ��������.
	for (int y = 0; y < image_height; y++) {
		// ���������� �����.
		for (int x = 0; x < image_width; x++) {
			double signal = 0.0;
			// ������������ ��������� �������.
			for (int i = 0; i < number_of_gaussian; i++) {
				double exp_expression_first = ((x - center_x[i]) * (x - center_x[i]));
				double exp_expression_second = ((y - center_y[i]) * (y - center_y[i]));
				signal += amplitude[i] * exp(-(exp_expression_first + exp_expression_second)/(2. * sigma_x[i]*sigma_y[i]));
			}
				
			// ������������ �������� �������.
			Pixel pix(x, y, signal);

			// ���������� ������� � ������ �����������.
			input_image.push_back(pix);
		}
	}

	input_image = calculate_color_of_image(input_image);
}

/**
��������� ���� �� �������� �����������.

@param noise_percent: ������� ���� %.
@return None
*/
void Image::get_noise_image(int noise_percent) {
	if (!input_image.empty()) {
		double noise_coefficient = noise_percent / 100.;

		// ������ ��������� �������������.
		vector<double> psi;
		for (int i = 0; i < input_image.size(); i++) {
			psi.push_back(calculate_psi());
		}

		double qpsi = 0.0;
		double signal_energy = 0.0;
		for (int i = 0; i < input_image.size(); i++) {
			// ������ ������� �������.
			signal_energy += input_image[i].value.re * input_image[i].value.re;

			// ������ ����� ��������� psi.
			qpsi += psi[i] * psi[i];
		}

		// ����������� ����.
		double alpha = sqrt(noise_coefficient * signal_energy / qpsi);

		// ���������� ������� � �����.
		noise_image = input_image;
		for (int i = 0; i < noise_image.size(); i++) {
			noise_image[i].value.re = input_image[i].value.re + alpha * psi[i];
		}

		noise_image = calculate_color_of_image(noise_image);
	}
}

/**
���������� ����� �� ��������� �����������.

@return None
*/
vector<Pixel> Image::calculate_color_of_image(vector<Pixel> image_vec) {
	if (!image_vec.empty()) {
		// ��������� �������� �����������.
		vector<double> values;
		for (int i = 0; i < image_vec.size(); i++) {
			values.push_back(image_vec[i].value.re);
		}

		// ��������� ������������� / ������������ ��������.
		double smallest_element = *min_element(values.begin(), values.end());
		double largest_element = *max_element(values.begin(), values.end());

		// ���������� ����� ������� �� ��� ��������.
		for (int i = 0; i < image_vec.size(); i++) {
			int color = image_vec[i].get_color_from_value(smallest_element, largest_element);
			image_vec[i].color_value = color;
		}
		return image_vec;
	}
}

/**
��������� �����������.

@return None
*/
void Image::draw_image(vector<Pixel> image, DrawSettings ds) {
	if (!image.empty() && ds._pic_wnd != NULL) {

		// ���������������.
		ds.xmin = 0;
		ds.xmax = image_width;
		ds.ymin = 0;
		ds.ymax = image_height;

		CBitmap bmp;
		CDC* MemDc;
		MemDc = new CDC;
		MemDc->CreateCompatibleDC(ds._pic_dc);

		double window_signal_width = ds._pic.Width();
		double window_signal_height = ds._pic.Height();
		ds.xp = (window_signal_width / (ds.xmax - ds.xmin));
		ds.yp = -(window_signal_height / (ds.ymax - ds.ymin));

		bmp.CreateCompatibleBitmap(ds._pic_dc, window_signal_width, window_signal_height);
		CBitmap* pBmp = (CBitmap*)MemDc->SelectObject(&bmp);
		
		// ������� ���� ������ ������.
		MemDc->FillSolidRect(ds._pic, RGB(0, 0, 0));

		// ��������� �����������.
		for (int i = 0; i < image.size(); i++) {
			// ��������� �������� ���������.
			pair<double, double> coords = ds.get_screen_coordinates(image[i].x, image[i].y);
			COLORREF color = RGB(image[i].color_value, image[i].color_value, image[i].color_value);

			double x_rect = 1;
			double y_rect = 1;
			MemDc->FillSolidRect((int)coords.first, (int)coords.second, (int)x_rect, (int)y_rect, color);
		}

		// ����� �� �����.
		ds._pic_dc->BitBlt(0, 0, window_signal_width, window_signal_height, MemDc, 0, 0, SRCCOPY);
		delete MemDc;
	}
	else {
		// ������� ���� ������ ������.
		ds._pic_dc->FillSolidRect(ds._pic, RGB(240, 240, 240));
	}
}

/**
	������� ���������� �����������.

	@param image_vec: ������ �������� ���������� ����������� ��� �������.
	@return ��������� ������.
	*/
vector<Pixel> Image::clear_selected_image(vector<Pixel> image_vec) {
	image_vec.clear();
	return image_vec;
}