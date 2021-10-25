#pragma once
#include "pch.h"
#include "panel_logic.h"
#include <fstream>
using namespace std;

/**
Файл для реализации логики работы программы.
*/


using namespace std;

// ****************************** Default ******************************

/**
Параметры изображения по умолчанию.
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

// Конструктор по умолчанию.
Pixel::Pixel() {
	x = 0.0;
	y = 0.0;
	value.re = 0.0;
	value.im = 0.0;
	color_value = 0;
}

/**
Конструктор с тремя параметрами.

@param in_x: Координата "x" пикселя.
@param in_y: Координата "y" пикселя.
@param in_value: Значение пикселя.
*/
Pixel::Pixel(double in_x, double in_y, double in_value) {
	x = in_x;
	y = in_y;
	value.re = in_value;
	value.im = 0.0;
	color_value = 0;
}

/**
Перевод значения пикселя в цвет.

@param min_value: Минимальное значение пикселя в изображении.
@param max_value: Максимальное значение пикселя в изображении.
@return None.
*/
int Pixel::get_color_from_value(double min_value, double max_value) {
	// Диапазон значений сигнала на каждый цвет.
	double color_range = (max_value - min_value) / 256.;

	vector<double> help;
	for (int i = 0; i <= 255; i++) {
		double limit_value = color_range * i + color_range;
		if (value.re <= limit_value) return i;
	}
}

/**
Вычисление модуля комплексного числа.

@return Модуль числа.
*/
double Pixel::calc_module_of_cmplx() {
	return sqrt(value.re * value.re + value.im * value.im);
}

// ****************************** DrawSettings ******************************

/**
Конструктор по умолчанию.
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
Конструктор с инициализацией параметров.
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
Перевод мировых координат в экранные.

@param world_x: Значение x в "мировых" координатах.
@papram world_y: Значение y в "мировых" координатах.
@return Пара значений (x, y) в "экранных" координатах.
*/
pair<double, double> DrawSettings::get_screen_coordinates(double world_x, double world_y) {
	pair<double, double> screen_pixel;
	screen_pixel.first = xp * (world_x - xmin);
	screen_pixel.second = yp * (world_y - ymax);
	return screen_pixel;
}

// ****************************** Image ******************************

/**
Конструктор по умолчанию.
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
Конструктор с параметрами.

@param is_model: Сгенерировать модель в качестве исходного изображения, если True.
	Загрузить существующее изображение в качестве исходного, если False.
@param width: Ширина изображения.
@param height: Высота изображения.
@param ds_input: Параметры рисования для исходного изображения.
@param ds_noise: Параметры рисования для зашумленного изображения.
@return None
*/
Image::Image(bool is_model, int width, int height, DrawSettings ds_in, DrawSettings ds_noi) {
	update_image_condition(is_model, width, height);
	ds_input = ds_in;
	ds_noise = ds_noi;
}

/**
	Обновление состояния (параметров) объекта.

	@param is_model: Сгенерировать модель в качестве исходного изображения, если True.
		Загрузить существующее изображение в качестве исходного, если False.
	@param ds: Параметры рисования.
	@param width: Ширина изображения.
	@param height: Высота изображения.
	@return None
	*/
void Image::update_image_condition(bool is_model, int width, int height) {
	_is_model_image = is_model;
	image_height = height;
	image_width = width;
}

/**
Генерация изображения в виде суммы Гауссовых куполов.

@return None
*/
void Image::get_input_image() {
	// Число Гауссовых куполов.
	int number_of_gaussian = 3;
	// Параметры сигнала.
	vector<double> amplitude{ 1, 1.5, 2 };
	vector<double> sigma_x{ ds_input.width * 0.1, ds_input.width * 0.09, ds_input.width * 0.06 };
	vector<double> sigma_y{ ds_input.height * 0.12, ds_input.height * 0.08, ds_input.height * 0.07 };
	vector<double> center_x{ 0.1 * ds_input.width, 0.5 * ds_input.width, 0.8 * ds_input.width };
	vector<double> center_y{ 0.2 * ds_input.height, 0.5 * ds_input.height, 0.75 * ds_input.height };

	// Заполнение столбцов.
	for (int y = 0; y < image_height; y++) {
		// Заполнение строк.
		for (int x = 0; x < image_width; x++) {
			double signal = 0.0;
			// Суперпозиция Гауссовых функций.
			for (int i = 0; i < number_of_gaussian; i++) {
				double exp_expression_first = ((x - center_x[i]) * (x - center_x[i]));
				double exp_expression_second = ((y - center_y[i]) * (y - center_y[i]));
				signal += amplitude[i] * exp(-(exp_expression_first + exp_expression_second)/(2. * sigma_x[i]*sigma_y[i]));
			}
				
			// Формирование значение пикселя.
			Pixel pix(x, y, signal);

			// Добавление пикселя в вектор изображения.
			input_image.push_back(pix);
		}
	}

	input_image = calculate_color_of_image(input_image);
}

/**
Наложение шума на исходное изображение.

@param noise_percent: Процент шума %.
@return None
*/
void Image::get_noise_image(int noise_percent) {
	if (!input_image.empty()) {
		double noise_coefficient = noise_percent / 100.;

		// Расчет случайных коэффициентов.
		vector<double> psi;
		for (int i = 0; i < input_image.size(); i++) {
			psi.push_back(calculate_psi());
		}

		double qpsi = 0.0;
		double signal_energy = 0.0;
		for (int i = 0; i < input_image.size(); i++) {
			// Расчет энергии сигнала.
			signal_energy += input_image[i].value.re * input_image[i].value.re;

			// Расчет суммы квадратов psi.
			qpsi += psi[i] * psi[i];
		}

		// Коэффициент шума.
		double alpha = sqrt(noise_coefficient * signal_energy / qpsi);

		// Заполнение сигнала с шумом.
		noise_image = input_image;
		for (int i = 0; i < noise_image.size(); i++) {
			noise_image[i].value.re = input_image[i].value.re + alpha * psi[i];
		}

		noise_image = calculate_color_of_image(noise_image);
	}
}

/**
Вычисление цвета по значениям изображения.

@return None
*/
vector<Pixel> Image::calculate_color_of_image(vector<Pixel> image_vec) {
	if (!image_vec.empty()) {
		// Получение значений изображения.
		vector<double> values;
		for (int i = 0; i < image_vec.size(); i++) {
			values.push_back(image_vec[i].value.re);
		}

		// Получение максимального / минимального значения.
		double smallest_element = *min_element(values.begin(), values.end());
		double largest_element = *max_element(values.begin(), values.end());

		// Вычисление цвета пикселя по его значению.
		for (int i = 0; i < image_vec.size(); i++) {
			int color = image_vec[i].get_color_from_value(smallest_element, largest_element);
			image_vec[i].color_value = color;
		}
		return image_vec;
	}
}

/**
Отрисовка изображения.

@return None
*/
void Image::draw_image(vector<Pixel> image, DrawSettings ds) {
	if (!image.empty() && ds._pic_wnd != NULL) {

		// Масштабирование.
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
		
		// Заливка фона черным цветом.
		MemDc->FillSolidRect(ds._pic, RGB(0, 0, 0));

		// Отрисовка изображения.
		for (int i = 0; i < image.size(); i++) {
			// Получение экранных координат.
			pair<double, double> coords = ds.get_screen_coordinates(image[i].x, image[i].y);
			COLORREF color = RGB(image[i].color_value, image[i].color_value, image[i].color_value);

			double x_rect = 1;
			double y_rect = 1;
			MemDc->FillSolidRect((int)coords.first, (int)coords.second, (int)x_rect, (int)y_rect, color);
		}

		// Вывод на экран.
		ds._pic_dc->BitBlt(0, 0, window_signal_width, window_signal_height, MemDc, 0, 0, SRCCOPY);
		delete MemDc;
	}
	else {
		// Заливка фона черным цветом.
		ds._pic_dc->FillSolidRect(ds._pic, RGB(240, 240, 240));
	}
}

/**
	Очистка выбранного изображения.

	@param image_vec: Вектор пикселей выбранного изображения для очистки.
	@return Очищенный массив.
	*/
vector<Pixel> Image::clear_selected_image(vector<Pixel> image_vec) {
	image_vec.clear();
	return image_vec;
}