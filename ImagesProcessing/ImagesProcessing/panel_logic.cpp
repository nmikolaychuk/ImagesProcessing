#pragma once
#include "pch.h"
#include "panel_logic.h"
#include <fstream>
using namespace std;



/**
Файл для реализации логики работы программы.
*/

// ****************************** Default ******************************

/**
Параметры изображения по умолчанию.
*/
int DEFAULT_WIDTH = 600;
int DEFAULT_HEIGHT = 600;

double calculate_psi() {
	double r = 0;
	int count_of_calc = 12;
	for (int i = 0; i < count_of_calc; i++)
	{
		r += ((rand() % 100) / (100. * 1.0) * 2.) - 1.;		// [-1;1]
	}
	return r / count_of_calc;
}

void Complex::get_module_value() {
	mod = sqrt(re * re + im * im);
}

// ****************************** Pixel ******************************

// Конструктор по умолчанию.
Pixel::Pixel() {
	value.re = 0.0;
	value.im = 0.0;
	value.mod = 0.0;
	color_value = 0;
}

/**
Конструктор с тремя параметрами.

@param in_x: Координата "x" пикселя.
@param in_y: Координата "y" пикселя.
@param in_value: Значение пикселя.
*/
Pixel::Pixel(double in_value) {
	value.re = in_value;
	value.im = 0.0;
	value.mod = 0.0;
	color_value = 0;
}

/**
Перевод значения пикселя в цвет.

@param min_value: Минимальное значение пикселя в изображении.
@param max_value: Максимальное значение пикселя в изображении.
@return None.
*/
int Pixel::get_color_from_value(double min_value, double max_value, bool is_module) {
	// Диапазон значений сигнала на каждый цвет.
	double color_range = (max_value - min_value) / 256.;

	vector<double> help;
	for (int i = 0; i <= 255; i++) {
		double limit_value = color_range * i + color_range;
		if (is_module) {
			if (value.mod <= limit_value) return i;
		}
		else {
			if (value.re <= limit_value) return i;
		}
	}
}

/**
Вычисление модуля комплексного числа.

@return None
*/
void Pixel::calc_module_of_cmplx() {
	value.get_module_value();
}

// ****************************** DrawSettings ******************************

/**
Конструктор по умолчанию.
*/
DrawSettings::DrawSettings() {
	_pic_wnd = NULL;
	_pic_dc = NULL;
	_pic = NULL;

	xmin = 0.0;
	xmax = 1.0;
	ymin = 0.0;
	ymax = 1.0;
	xp = 0.0;
	yp = 0.0;
	width = DEFAULT_WIDTH;
	height = DEFAULT_HEIGHT;
	circle_window = false;
	rect_window = false;
	filtration_percent = 0;
	step_x = 1.0;
	step_y = 1.0;
}

/**
Конструктор с инициализацией параметров.
*/
DrawSettings::DrawSettings(CWnd* pic_wnd, CDC* pic_dc, CRect pic,
	double n_xmax, double n_ymax, bool is_crcl_wnd = false, bool is_rct_wnd = false, int filt_percent = 0) {
	_pic_wnd = pic_wnd;
	_pic_dc = pic_dc;
	_pic = pic;

	xmin = 0.0;
	xmax = 1.0;
	ymin = 0.0;
	ymax = 1.0;
	xp = 0.0;
	yp = 0.0;
	step_x = 1.0;
	step_y = 1.0;
	width = n_xmax;
	height = n_ymax;
	circle_window = is_crcl_wnd;
	rect_window = is_rct_wnd;
	filtration_percent = filt_percent;
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
	ds_spec = *dset;
	ds_filtered = *dset;
	_is_zeros_addition = true;
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
Image::Image(bool is_model, bool is_zeros_add, int width, int height, DrawSettings ds_in,
			 DrawSettings ds_noi, DrawSettings ds_sp, DrawSettings ds_ft) {
	update_image_condition(is_model, is_zeros_add, width, height);
	ds_input = ds_in;
	ds_noise = ds_noi;
	ds_spec = ds_sp;
	ds_filtered = ds_ft;
	window_offset_x = 0;
	window_offset_y = 0;
	circle_radius = 1;
	_is_zeros_addition = is_zeros_add;
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
void Image::update_image_condition(bool is_model, bool is_zeros_add, int width, int height) {
	_is_model_image = is_model;
	_is_zeros_addition = is_zeros_add;
	image_height = height;
	image_width = width;
	window_offset_x = 0;
	window_offset_y = 0;
	circle_radius = 1;
}

/**
	Вычислений модулей каждой точки в переданном изображении.

	@return None
	*/
void Image::calculate_modules(vector<vector<Pixel>>& data) {
	for (int i = 0; i < data.size(); i++) {
		for (int j = 0; j < data[i].size(); j++) {
			data[i][j].calc_module_of_cmplx();
		}
	}
}

/**
Генерация изображения в виде суммы Гауссовых куполов.

@return None
*/
void Image::get_input_image() {
	input_image.clear();
	// Число Гауссовых куполов.
	int number_of_gaussian = 3;
	// Параметры сигнала.
	vector<double> amplitude{ 0, 2, 0 };
	vector<double> sigma_x{ image_width * 0.003, image_width * 0.3, image_width * 0.003 };
	vector<double> sigma_y{ image_height * 0.003, image_height * 0.3, image_height * 0.003 };
	vector<double> center_x{ 0.1 * image_width, 0.5 * image_width, 0.505 * image_width };
	vector<double> center_y{ 0.2 * image_height, 0.5 * image_height, 0.495 * image_height };

	// Заполнение столбцов.
	for (int y = 0; y < image_height; y++) {
		// Заполнение строк.
		vector<Pixel> row;
		for (int x = 0; x < image_width; x++) {
			double signal = 0.0;
			// Суперпозиция Гауссовых функций.
			for (int i = 0; i < number_of_gaussian; i++) {
				double exp_expression_first = ((x - center_x[i]) * (x - center_x[i]));
				double exp_expression_second = ((y - center_y[i]) * (y - center_y[i]));
				signal += amplitude[i] * exp(-(exp_expression_first + exp_expression_second) / (2. * sigma_x[i] * sigma_y[i]));
			}

			// Формирование значение пикселя.
			Pixel pix(signal);

			// Добавление пикселя в вектор изображения.
			row.push_back(pix);
		}
		input_image.push_back(row);
	}

	// Вычисление модулей.
	calculate_modules(input_image);

	// Вычисление цветов.
	input_image = calculate_color_of_image(input_image, false);
}

/**
Наложение шума на исходное изображение.

@param noise_percent: Процент шума %.
@return None
*/
void Image::get_noise_image(int noise_percent) {
	if (!input_image.empty()) {

		// Нормировка входного сигнала.
		double energy_re = 0.0;
		for (int i = 0; i < input_image.size(); i++) {
			for (int j = 0; j < input_image[i].size(); j++) {
				energy_re += input_image[i][j].value.re * input_image[i][j].value.re;
			}
		}

		for (int i = 0; i < input_image.size(); i++) {
			for (int j = 0; j < input_image[i].size(); j++) {
				input_image[i][j].value.re /= energy_re;
				input_image[i][j].calc_module_of_cmplx();
			}
		}
		input_image = calculate_color_of_image(input_image, false);

		double noise_coefficient = noise_percent / 100.;

		// Расчет случайных коэффициентов.
		vector<vector<double>> psi;
		for (int i = 0; i < input_image.size(); i++) {
			vector<double> temp;
			for (int j = 0; j < input_image[i].size(); j++) {
				temp.push_back(calculate_psi());
			}
			psi.push_back(temp);
		}

		double qpsi = 0.0;
		double signal_energy = 0.0;
		for (int i = 0; i < input_image.size(); i++) {
			for (int j = 0; j < input_image[i].size(); j++) {
				// Расчет энергии сигнала.
				signal_energy += input_image[i][j].value.re * input_image[i][j].value.re;

				// Расчет суммы квадратов psi.
				qpsi += psi[i][j] * psi[i][j];
			}
		}

		// Коэффициент шума.
		double alpha = sqrt(noise_coefficient * signal_energy / qpsi);

		// Заполнение сигнала с шумом.
		noise_image = input_image;
		for (int i = 0; i < noise_image.size(); i++) {
			for (int j = 0; j < noise_image[i].size(); j++) {
				noise_image[i][j].value.re = input_image[i][j].value.re + alpha * psi[i][j];
			}
		}

		calculate_modules(noise_image);
		noise_image = calculate_color_of_image(noise_image, false);
	}
}

void fourea(vector<Pixel>& data, int n, int is)
{
	int i, j, istep;
	int m, mmax;
	double r, r1, theta, w_r, w_i, temp_r, temp_i;
	double pi = 3.1415926;

	r = pi * is;
	j = 0;
	for (i = 0; i < n; i++)
	{
		if (i < j)
		{
			temp_r = data[j].value.re;
			temp_i = data[j].value.im;
			data[j].value.re = data[i].value.re;
			data[j].value.im = data[i].value.im;
			data[i].value.re = temp_r;
			data[i].value.im = temp_i;
		}
		m = n >> 1;
		while (j >= m) { j -= m; m = (m + 1) / 2; }
		j += m;
	}
	mmax = 1;
	while (mmax < n)
	{
		istep = mmax << 1;
		r1 = r / (double)mmax;
		for (m = 0; m < mmax; m++)
		{
			theta = r1 * m;
			w_r = (double)cos((double)theta);
			w_i = (double)sin((double)theta);
			for (i = m; i < n; i += istep)
			{
				j = i + mmax;
				temp_r = w_r * data[j].value.re - w_i * data[j].value.im;
				temp_i = w_r * data[j].value.im + w_i * data[j].value.re;
				data[j].value.re = data[i].value.re - temp_r;
				data[j].value.im = data[i].value.im - temp_i;
				data[i].value.re += temp_r;
				data[i].value.im += temp_i;
			}
		}
		mmax = istep;
	}
	if (is > 0)
		for (i = 0; i < n; i++)
		{
			data[i].value.re /= (double)n;
			data[i].value.im /= (double)n;
		}

}

/**
Быстрое преобразование Фурье.

@param data: Вектор изображения.
@param is: Флаг, определяющий тип преобразования: -1 - ППФ; 1 - ОПФ.
@return None
*/
void fourea_transform(vector<vector<Pixel>>& data, int is)
{
	// Проход по всем строкам.
	for (int i = 0; i < data.size(); i++) {
		fourea(data[i], data[i].size(), is);
	}

	// Проход по всем столбцам.
	for (int j = 0; j < data[0].size(); j++) {
		vector<Pixel> buffer;
		for (int i = 0; i < data.size(); i++) {
			buffer.push_back(data[i][j]);
		}

		fourea(buffer, buffer.size(), is);

		for (int i = 0; i < data.size(); i++) {
			data[i][j] = buffer[i];
		}
	}
}

/**
	Поиск ближайшей степени двойки у числа.
	*/
long clp2(long x) {
	x--;
	for (int p = 1; p < 32; p <<= 1) x |= (x >> p);
	return ++x;
}

/**
	Линейная интерполяция вектора.
*/
vector<Pixel> Image::vector_interpolation(vector<Pixel> vec, int new_size, bool is_width) {
	int old_size = vec.size();
	vector<Pixel> new_vec;
	double new_step = ((double)old_size / (double)new_size);
	
	new_vec.push_back(vec[0]);
	for (double i = new_step; i < old_size; i += new_step) {
		int left_bound = (int)i;
		int right_bound = left_bound + 1;
		if (new_size > old_size) {
			if (right_bound >= old_size) {
				break;
			}
		}
		else {
			if (right_bound >= old_size-1) {
				break;
			}
		}
		Pixel new_pixel;

		// Вычисление нового значения реальной части.
		double upper_re = vec[right_bound].value.re - vec[left_bound].value.re;
		double lower = right_bound - left_bound;
		new_pixel.value.re = vec[left_bound].value.re + (i - left_bound) * (upper_re / lower);

		// Вычисление нового значения мнимой части.
		double upper_im = vec[right_bound].value.im - vec[left_bound].value.im;
		new_pixel.value.im = vec[left_bound].value.im + (i - left_bound) * (upper_im / lower);

		new_vec.push_back(new_pixel);
	}
	new_vec.push_back(vec.back());

	// Вычисление модулей каждого пикселя.
	for (int i = 0; i < new_vec.size(); i++) {
		new_vec[i].calc_module_of_cmplx();
	}

	if (is_width) {
		ds_spec.step_x = new_step;
	}
	else {
		ds_spec.step_y = new_step;
	}

	return new_vec;
}

/**
	Доопределение зашумленного изображения до размера степени двойки.
	Используется строго перед преобразованием Фурье.

	@return None
	*/
void Image::get_addition_image() {
	// Вычисление ближайших степеней двойки.
	int correct_width = clp2(input_image[0].size());
	int correct_height = clp2(input_image.size());
	ds_spec.height = correct_height;
	ds_spec.width = correct_width;

	vector<vector<Pixel>> temp = spectrum_image_calc;

	// Очистка и изменение размера.
	spectrum_image_calc.clear();
	spectrum_image_calc.resize(correct_height);
	for (int i = 0; i < correct_height; i++) {
		spectrum_image_calc[i].resize(correct_width);
	}

	if (_is_zeros_addition) {
		// Добавляем старые значения к расширенному изображению.
		for (int y = 0; y < temp.size(); y++) {
			for (int x = 0; x < temp[y].size(); x++) {
				spectrum_image_calc[y][x] = temp[y][x];
			}
		}
	}
	else {
		vector<vector<Pixel>> after_width_addition;
		// Выполнение линейной интерполяции по строкам.
		for (int i = 0; i < temp.size(); i++) {
			after_width_addition.push_back(vector_interpolation(temp[i], correct_width, true));
		}

		// Выполнение линейной интерполяции по столбцам.
		for (int j = 0; j < after_width_addition[0].size(); j++) {
			vector<Pixel> buffer;
			for (int i = 0; i < after_width_addition.size(); i++) {
				buffer.push_back(after_width_addition[i][j]);
			}

			buffer = vector_interpolation(buffer, correct_height, false);

			for (int i = 0; i < spectrum_image_calc.size(); i++) {
				spectrum_image_calc[i][j] = buffer[i];
			}
		}
	}
}

/**
	Вычисление спектра изображения.

	@return None
	*/
void Image::get_spectrum_image() {
	if (!noise_image.empty() && !input_image.empty()) {
		spectrum_image_calc = noise_image;
		
		// Дополнение размера до степени двойки для БПФ.
		get_addition_image();

		// Прямое преобразование Фурье.
		fourea_transform(spectrum_image_calc, -1);

		// Вычисление энергии сигнала.
		double signal_energy = 0.0;
		for (int i = 0; i < spectrum_image_calc.size(); i++) {
			for (int j = 0; j < spectrum_image_calc[i].size(); j++) {
				signal_energy += spectrum_image_calc[i][j].value.mod * spectrum_image_calc[i][j].value.mod;
			}
		}

		calculate_modules(spectrum_image_calc);
		spectrum_image_calc = calculate_color_of_image(spectrum_image_calc, true);

		// Подготовка изображения для отрисовки.
		spectrum_image_vis.resize(spectrum_image_calc.size());
		for (int i = 0; i < spectrum_image_vis.size(); i++) {
			spectrum_image_vis[i].resize(spectrum_image_calc[i].size());
		}

		// Перестановка четвертей.
		int w_on_2 = (int)(spectrum_image_vis[0].size() / 2);
		int h_on_2 = (int)(spectrum_image_vis.size() / 2);
		for (int y = h_on_2; y < spectrum_image_vis.size(); y++) {
			for (int x = w_on_2; x < spectrum_image_vis[0].size(); x++) {
				spectrum_image_vis[y][x] = spectrum_image_calc[y - h_on_2][x - w_on_2];
				if (y == h_on_2 && x == w_on_2) {
					spectrum_image_vis[y][x].value.im = 0.0;
					spectrum_image_vis[y][x].value.re = 0.0;
					spectrum_image_vis[y][x].value.mod = 0.0;
				}
			}
		}

		for (int y = 0; y < h_on_2; y++) {
			for (int x = 0; x < w_on_2; x++) {
				spectrum_image_vis[y][x] = spectrum_image_calc[y + h_on_2][x + w_on_2];
				if (y == h_on_2 - 1 && x == w_on_2 - 1) {
					spectrum_image_vis[y][x].value.im = 0.0;
					spectrum_image_vis[y][x].value.re = 0.0;
					spectrum_image_vis[y][x].value.mod = 0.0;
				}
			}
		}

		for (int y = 0; y < h_on_2; y++) {
			for (int x = w_on_2; x < spectrum_image_vis[0].size(); x++) {
				spectrum_image_vis[y][x] = spectrum_image_calc[y + h_on_2][x - w_on_2];
				if (y == 0 && x == image_width - 1) {
					spectrum_image_vis[y][x].value.im = 0.0;
					spectrum_image_vis[y][x].value.re = 0.0;
					spectrum_image_vis[y][x].value.mod = 0.0;
				}
			}
		}

		for (int y = h_on_2; y < spectrum_image_vis.size(); y++) {
			for (int x = 0; x < w_on_2; x++) {
				spectrum_image_vis[y][x] = spectrum_image_calc[y - h_on_2][x + w_on_2];
				if (y == image_height - 1 && x == 0) {
					spectrum_image_vis[y][x].value.im = 0.0;
					spectrum_image_vis[y][x].value.re = 0.0;
					spectrum_image_vis[y][x].value.mod = 0.0;
				}
			}
		}

		// Нормировка на энергию сигнала.
		//for (int i = 0; i < spectrum_image_vis.size(); i++) {
		//	for (int j = 0; j < spectrum_image_vis[i].size(); j++) {
		//		spectrum_image_vis[i][j].value.mod /= signal_energy;
		//	}
		//}


		spectrum_image_vis = calculate_color_of_image(spectrum_image_vis, true);
	}
}

/**
	Фильтрация изображения.

	@return None
	*/
void Image::get_filtered_image() {
	if (!inverted_spectrum_quanters.empty() && !input_image.empty() && !noise_image.empty() && !spectrum_image_calc.empty()) {
		vector<vector<Pixel>> quanter_1 = inverted_spectrum_quanters[0];
		vector<vector<Pixel>> quanter_2 = inverted_spectrum_quanters[1];
		vector<vector<Pixel>> quanter_3 = inverted_spectrum_quanters[2];
		vector<vector<Pixel>> quanter_4 = inverted_spectrum_quanters[3];
		int w_on_2 = spectrum_image_calc[0].size()/2;
		int h_on_2 = spectrum_image_calc.size()/2;
		int center_x = 0;
		int center_y = 0;

		// Обработка прямоугольного окна.
		if (ds_spec.rect_window) {
			for (int i = 0; i < h_on_2; i++) {
				for (int j = 0; j < w_on_2; j++) {
					if (i >= window_offset_y || j >= window_offset_x) {
						quanter_1[i][j].value.re = 0.0;
						quanter_1[i][j].value.im = 0.0;
						quanter_1[i][j].value.mod = 0.0;
						quanter_1[i][j].color_value = 0;

						quanter_2[i][j].value.re = 0.0;
						quanter_2[i][j].value.im = 0.0;
						quanter_2[i][j].value.mod = 0.0;
						quanter_2[i][j].color_value = 0;

						quanter_3[i][j].value.re = 0.0;
						quanter_3[i][j].value.im = 0.0;
						quanter_3[i][j].value.mod = 0.0;
						quanter_3[i][j].color_value = 0;

						quanter_4[i][j].value.re = 0.0;
						quanter_4[i][j].value.im = 0.0;
						quanter_4[i][j].value.mod = 0.0;
						quanter_4[i][j].color_value = 0;
					}
				}
			}
		}
		// Обработка круглого окна.
		if (ds_spec.circle_window) {
			for (int i = 0; i < h_on_2; i++) {
				for (int j = 0; j < w_on_2; j++) {
					double in_circle_condition = (i - center_y) * (i - center_y) + (j - center_x) * (j - center_x);
					if (in_circle_condition > circle_radius * circle_radius) {
						quanter_1[i][j].value.re = 0.0;
						quanter_1[i][j].value.im = 0.0;
						quanter_1[i][j].value.mod = 0.0;
						quanter_1[i][j].color_value = 0;

						quanter_2[i][j].value.re = 0.0;
						quanter_2[i][j].value.im = 0.0;
						quanter_2[i][j].value.mod = 0.0;
						quanter_2[i][j].color_value = 0;

						quanter_3[i][j].value.re = 0.0;
						quanter_3[i][j].value.im = 0.0;
						quanter_3[i][j].value.mod = 0.0;
						quanter_3[i][j].color_value = 0;

						quanter_4[i][j].value.re = 0.0;
						quanter_4[i][j].value.im = 0.0;
						quanter_4[i][j].value.mod = 0.0;
						quanter_4[i][j].color_value = 0;
					}
				}
			}
		}

		vector<vector<Pixel>> new_quanter_1(h_on_2, vector<Pixel>(w_on_2, Pixel()));
		vector<vector<Pixel>> new_quanter_2(h_on_2, vector<Pixel>(w_on_2, Pixel()));
		vector<vector<Pixel>> new_quanter_3(h_on_2, vector<Pixel>(w_on_2, Pixel()));
		vector<vector<Pixel>> new_quanter_4(h_on_2, vector<Pixel>(w_on_2, Pixel()));

		filtered_image.clear();
		filtered_image.resize(spectrum_image_calc.size(), vector<Pixel>(spectrum_image_calc[0].size(), Pixel()));

		// Инвертирование кванторов.
		for (int i = 0; i < h_on_2; i++) {
			for (int j = 0; j < w_on_2; j++) {
				new_quanter_1[i][j] = quanter_1[h_on_2 - 1 - i][w_on_2 - 1 - j];
				new_quanter_2[i][j] = quanter_2[h_on_2 - 1 - i][j];
				new_quanter_3[i][j] = quanter_3[i][j];
				new_quanter_4[i][j] = quanter_4[i][w_on_2 - 1 - j];
			}
		}

		// Склеивание кванторов в целое изображение.
		for (int i = 0; i < spectrum_image_calc.size(); i++) {
			for (int j = 0; j < spectrum_image_calc[i].size(); j++) {
				if (i < h_on_2 && j < w_on_2) {
					filtered_image[i][j] = new_quanter_3[i][j];
				}

				if (i >= h_on_2 && j < w_on_2) {
					filtered_image[i][j] = new_quanter_2[i - h_on_2][j];
				}

				if (i < h_on_2 && j >= w_on_2) {
					filtered_image[i][j] = new_quanter_4[i][j - w_on_2];
				}

				if (i >= h_on_2 && j >= w_on_2) {
					filtered_image[i][j] = new_quanter_1[i - h_on_2][j - w_on_2];
				}
			}
		}

		// ОПФ.
		fourea_transform(filtered_image, 1);

		// Удаление добавки размера.
		vector<vector<Pixel>> temp = filtered_image;
		filtered_image.clear();
		filtered_image.resize(input_image.size(), vector<Pixel>(input_image[0].size(), Pixel()));

		if (_is_zeros_addition) {
			for (int i = 0; i < input_image.size(); i++) {
				for (int j = 0; j < input_image[0].size(); j++) {
					filtered_image[i][j] = temp[i][j];
				}
			}
		}
		else {
			vector<vector<Pixel>> after_width_addition;
			// Выполнение линейной интерполяции по строкам.
			for (int i = 0; i < temp.size(); i++) {
				after_width_addition.push_back(vector_interpolation(temp[i], input_image[0].size(), true));
			}

			// Выполнение линейной интерполяции по столбцам.
			for (int j = 0; j < after_width_addition[0].size(); j++) {
				vector<Pixel> buffer;
				for (int i = 0; i < after_width_addition.size(); i++) {
					buffer.push_back(after_width_addition[i][j]);
				}

				buffer = vector_interpolation(buffer, input_image.size(), false);

				for (int i = 0; i < filtered_image.size(); i++) {
					filtered_image[i][j] = buffer[i];
				}
			}
		}

		calculate_modules(filtered_image);
		filtered_image = calculate_color_of_image(filtered_image, false);
	}
}

/**
Вычисление цвета по значениям изображения.

@return None
*/
vector<vector<Pixel>> Image::calculate_color_of_image(vector<vector<Pixel>> image_vec, bool is_module = false) {
	if (!image_vec.empty()) {
		// Получение значений изображения.
		vector<double> values;
		for (int i = 0; i < image_vec.size(); i++) {
			for (int j = 0; j < image_vec[i].size(); j++) {
				if (is_module) {
					values.push_back(image_vec[i][j].value.mod);
				}
				else {
					values.push_back(image_vec[i][j].value.re);
				}
			}
		}

		// Получение максимального / минимального значения.
		double smallest_element = *min_element(values.begin(), values.end());
		double largest_element = *max_element(values.begin(), values.end());

		// Вычисление цвета пикселя по его значению.
		for (int i = 0; i < image_vec.size(); i++) {
			for (int j = 0; j < image_vec[i].size(); j++) {
				int color = image_vec[i][j].get_color_from_value(smallest_element, largest_element, is_module);
				image_vec[i][j].color_value = color;
			}
		}
		return image_vec;
	}
}

/**
Отрисовка изображения.

@return None
*/
void Image::draw_image(vector<vector<Pixel>> image, DrawSettings ds, bool draw_window = false) {
	if (!image.empty() && ds._pic_wnd != NULL) {

		// Масштабирование.
		ds.xmin = 0;
		ds.xmax = image[0].size();
		ds.ymin = 0;
		ds.ymax = image.size();

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
			for (int j = 0; j < image[i].size(); j++) {
				// Получение экранных координат.
				pair<double, double> coords = ds.get_screen_coordinates(j*ds.step_x, i*ds.step_y);
				COLORREF color = RGB(image[i][j].color_value, image[i][j].color_value, image[i][j].color_value);


				double x_rect = 6;
				double y_rect = 6;
				if (ds.width < 256) {
					x_rect = 8;
				}
				if (ds.height < 256) {
					y_rect = 8;
				}
				MemDc->FillSolidRect((int)coords.first, (int)coords.second, (int)x_rect, (int)y_rect, color);
			}
		}

		if (draw_window) {
			window_offset_x = 0;
			window_offset_y = 0;
			inverted_spectrum_quanters.clear();
			if (!spectrum_image_calc.empty() && ds.filtration_percent > 0) {
				int w = image[0].size() / 2;
				int h = image.size() / 2;
				pair<double, double> center = ds.get_screen_coordinates(w, h);

				// Создание инструментов для рисования функции окна.
				CBrush brush;
				brush.CreateStockObject(HOLLOW_BRUSH);
				CPen pen;
				pen.CreatePen(PS_DOT, 1, RGB(255, 0, 0));

				// Выделение квадрантов в отдельные массивы у рассчитанного спектра (НЕ У МАССИВА ДЛЯ ОТРИСОВКИ).
				int h_on_2 = spectrum_image_calc.size() / 2;
				int w_on_2 = spectrum_image_calc[0].size() / 2;
				Pixel init;
				init.value.re = 0.0;
				init.value.im = 0.0;
				init.value.mod = 0.0;
				init.color_value = 0;
				vector<vector<Pixel>> quanter_1(h_on_2, vector<Pixel>(w_on_2, init));
				vector<vector<Pixel>> quanter_2(h_on_2, vector<Pixel>(w_on_2, init));
				vector<vector<Pixel>> quanter_3(h_on_2, vector<Pixel>(w_on_2, init));
				vector<vector<Pixel>> quanter_4(h_on_2, vector<Pixel>(w_on_2, init));
				for (int i = 0; i < spectrum_image_calc.size(); i++) {
					for (int j = 0; j < spectrum_image_calc[i].size(); j++) {
						if (i < h_on_2 && j < w_on_2) {
							quanter_3[i][j] = spectrum_image_calc[i][j];
						}

						if (i < h_on_2 && j >= w_on_2) {
							quanter_4[i][j - w_on_2] = spectrum_image_calc[i][j];
						}

						if (i >= h_on_2 && j < w_on_2) {
							quanter_2[i - h_on_2][j] = spectrum_image_calc[i][j];
						}

						if (i >= h_on_2 && j >= w_on_2) {
							quanter_1[i - h_on_2][j - w_on_2] = spectrum_image_calc[i][j];
						}
					}
				}

				// Инвертирование квадрантов так, чтобы пик был в начале координат (слева внизу).
				vector<vector<Pixel>> inverted_quanter_1(h_on_2, vector<Pixel>(w_on_2, init));
				vector<vector<Pixel>> inverted_quanter_2(h_on_2, vector<Pixel>(w_on_2, init));
				vector<vector<Pixel>> inverted_quanter_3 = quanter_3;
				vector<vector<Pixel>> inverted_quanter_4(h_on_2, vector<Pixel>(w_on_2, init));

				// 1 квадрант.
				for (int i = 0; i < quanter_1.size(); i++) {
					for (int j = 0; j < quanter_1[i].size(); j++) {
						inverted_quanter_1[i][j] = quanter_1[quanter_1.size() - 1 - i][quanter_1[quanter_1.size() - 1 - i].size() - 1 - j];
					}
				}

				// 2 квадрант.
				for (int i = 0; i < quanter_2.size(); i++) {
					for (int j = 0; j < quanter_2[i].size(); j++) {
						inverted_quanter_2[i][j] = quanter_2[quanter_2.size() - 1 - i][j];
					}
				}

				// 4 квадрант.
				for (int i = 0; i < quanter_4.size(); i++) {
					for (int j = 0; j < quanter_4[i].size(); j++) {
						inverted_quanter_4[i][j] = quanter_4[i][quanter_4[quanter_4.size() - 1 - i].size() - 1 - j];
					}
				}

				double signal_energy = 0.0;
				for (int i = 0; i < h_on_2; i++) {
					for (int j = 0; j < w_on_2; j++) {
						signal_energy += inverted_quanter_1[i][j].value.mod * inverted_quanter_1[i][j].value.mod +
							inverted_quanter_2[i][j].value.mod * inverted_quanter_2[i][j].value.mod +
							inverted_quanter_3[i][j].value.mod * inverted_quanter_3[i][j].value.mod +
							inverted_quanter_4[i][j].value.mod * inverted_quanter_4[i][j].value.mod;
					}
				}

				inverted_spectrum_quanters.push_back(inverted_quanter_1);
				inverted_spectrum_quanters.push_back(inverted_quanter_2);
				inverted_spectrum_quanters.push_back(inverted_quanter_3);
				inverted_spectrum_quanters.push_back(inverted_quanter_4);

				// Оставить процент фильтрации.
				int percent_of_signal = ds.filtration_percent;
				double current_energy = 0.0;
				int offset_x = 0;
				int offset_y = 0;
				int center_x = 0;
				int center_y = 0;
				MemDc->SelectObject(&pen);
				MemDc->SelectObject(&brush);

				if (ds.circle_window) {
					circle_radius = 1;
					while (current_energy < ((percent_of_signal / 100.) * signal_energy) && circle_radius < w_on_2 && circle_radius < h_on_2) {
						current_energy = 0.0;
						for (int i = 0; i < circle_radius; i++) {
							for (int j = 0; j < circle_radius; j++) {

								double in_circle_condition = (i - center_y) * (i - center_y) + (j - center_x) * (j - center_x);
								if (in_circle_condition <= circle_radius * circle_radius) {
									current_energy += inverted_quanter_1[i][j].value.mod * inverted_quanter_1[i][j].value.mod +
										inverted_quanter_2[i][j].value.mod * inverted_quanter_2[i][j].value.mod +
										inverted_quanter_3[i][j].value.mod * inverted_quanter_3[i][j].value.mod +
										inverted_quanter_4[i][j].value.mod * inverted_quanter_4[i][j].value.mod;
								}
								offset_x = j;
								offset_y = i;
							}
						}
						circle_radius += 1;
					}

					pair<double, double> screen_off = ds.get_screen_coordinates(offset_x, ds.ymax - offset_y);

					MemDc->Ellipse(center.first - screen_off.first, center.second - screen_off.second,
						center.first + screen_off.first, center.second + screen_off.second);

					window_offset_x = offset_x;
					window_offset_y = offset_y;
				}
				else if (ds.rect_window) {
					double rect_side = 1;
					while (current_energy < ((percent_of_signal / 100.) * signal_energy) && rect_side < w_on_2 && rect_side < h_on_2) {
						current_energy = 0.0;
						for (int i = 0; i < rect_side; i++) {
							for (int j = 0; j < rect_side; j++) {
								if (j <= rect_side && i <= rect_side) {
									current_energy += inverted_quanter_1[i][j].value.mod * inverted_quanter_1[i][j].value.mod +
										inverted_quanter_2[i][j].value.mod * inverted_quanter_2[i][j].value.mod +
										inverted_quanter_3[i][j].value.mod * inverted_quanter_3[i][j].value.mod +
										inverted_quanter_4[i][j].value.mod * inverted_quanter_4[i][j].value.mod;
								}
								offset_x = j;
								offset_y = i;
							}
						}
						rect_side += 1;
					}

					pair<double, double> screen_off = ds.get_screen_coordinates(offset_x, ds.ymax - offset_y);
					MemDc->Rectangle(center.first - screen_off.first, center.second - screen_off.second,
						center.first + screen_off.first, center.second + screen_off.second);

					window_offset_x = offset_x;
					window_offset_y = offset_y;
				}
			}
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
vector<vector<Pixel>> Image::clear_selected_image(vector<vector<Pixel>> image_vec) {
	for (int i = 0; i < image_vec.size(); i++) {
		image_vec[i].clear();
	}
	image_vec.clear();
	return image_vec;
}

/**
	Вычисление невязки для исходного и восстановленного сигнала.

	@return Невязка.
	*/
double Image::calculate_difference() {
	if (!input_image.empty() && !filtered_image.empty()) {
		double upper = 0.0;
		double lower = 0.0;

		for (int i = 0; i < input_image.size(); i++) {
			for (int j = 0; j < input_image[i].size(); j++) {
				//double mod_input = sqrt(input_image[i][j].value.re * input_image[i][j].value.re + input_image[i][j].value.im * input_image[i][j].value.im);
				//double mod_filt = sqrt(filtered_image[i][j].value.re * filtered_image[i][j].value.re + filtered_image[i][j].value.im * filtered_image[i][j].value.im);
				upper += (input_image[i][j].color_value - filtered_image[i][j].color_value) * (input_image[i][j].color_value - filtered_image[i][j].color_value);
				lower += input_image[i][j].color_value * input_image[i][j].color_value;
			}
		}

		double result = upper / lower;
		result = sqrt(result);

		return result;
	}
}