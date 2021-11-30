#pragma once
#include <math.h>
#include <algorithm>
#include <vector>
using namespace std;


/**
Реализация комплексного числа.

*/
class Complex {
public:
	double re;
	double im;
	double mod;

	void get_module_value();
};


/**
Определение точки изображения.

Имеются публичные поля:
x - координата x пикселя;
y - координата y пикселя;
value - значение пикселя (мировое, Complex);
color_value - значение пикселя (экранное, цвет).
*/
class Pixel {
public:
	Complex value;
	int color_value;

	/**
	Конструктор по умолчанию.
	*/
	Pixel();

	/**
	Конструктор с тремя параметрами.

	@param in_value: Значение пикселя.
	*/
	Pixel(double in_value);

	/**
	Перевод значения пикселя в цвет.

	@param min_value: Минимальное значение пикселя в изображении.
	@param max_value: Максимальное значение пикселя в изображении.
	@return Значение цвета [0-255].
	*/
	int get_color_from_value(double min_value, double max_value, bool is_module);

	/**
	Вычисление модуля комплексного числа.

	@return None
	*/
	void calc_module_of_cmplx();
};


/**
Параметры отрисовки.
*/
class DrawSettings {
public:
	// Параметры окна.
	CWnd* _pic_wnd;
	CDC* _pic_dc;
	CRect _pic;
	// Размеры изображения.
	double width, height;
	// Пересчет координат.
	double xmin, xmax, ymin, ymax;
	double xp, yp;
	bool circle_window;
	bool rect_window;
	int filtration_percent;
	double step_x;
	double step_y;

	/**
	Конструктор по умолчанию.
	*/
	DrawSettings();

	/**
	Конструктор с инициализацией параметров.
	*/
	DrawSettings(CWnd* pic_wnd, CDC* pic_dc, CRect pic, double n_xmax, double n_ymax,
				 bool is_crcl_wnd, bool is_rct_wnd, int filtration_percent);

	/**
	Перевод мировых координат в экранные.
	*/
	pair<double, double> get_screen_coordinates(double world_x, double world_y);
};


/**
Реализация исходного изображения.
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

	// Конструктор по умолчанию.
	Image();

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
	Image(bool is_model, bool _is_zeros_addition, int width, int height, DrawSettings ds_in,
		  DrawSettings ds_noi, DrawSettings ds_sp, DrawSettings ds_ft);

	/**
	Обновление состояния (параметров) объекта.

	@param is_model: Сгенерировать модель в качестве исходного изображения, если True.
		Загрузить существующее изображение в качестве исходного, если False.
	@param ds: Параметры рисования.
	@param width: Ширина изображения.
	@param height: Высота изображения.
	@return None
	*/
	void update_image_condition(bool is_model, bool is_zeros_add, int width, int height);

	/**
	Генерация изображения в виде суммы Гауссовых куполов.

	@return None
	*/
	void get_input_image();

	/**
	Наложение шума на исходное изображение.

	@param noise_percent: Процент шума %.
	@return None
	*/
	void get_noise_image(int noise_percent);

	/**
	Вычисление спектра изображения.

	@return None
	*/
	void get_spectrum_image();

	/**
	Фильтрация изображения.

	@return None
	*/
	void get_filtered_image();

	/**
	Линейная интерполяция вектора.
	*/
	vector<Pixel> vector_interpolation(vector<Pixel> vec, int new_size, bool is_width);

	/**
	Доопределение исходного изображения до размера степени двойки.

	@return None
	*/
	void get_addition_image();

	/**
	Отрисовка изображения.
	
	@return None
	*/
	void draw_image(vector<vector<Pixel>> image, DrawSettings ds, bool draw_window);

	/**
	Вычисление цвета по значениям изображения.

	@return Изображение с вычесленными цветами.
	*/
	vector<vector<Pixel>> calculate_color_of_image(vector<vector<Pixel>> image_vec, bool is_module);

	/**
	Очистка выбранного изображения.

	@param image_vec: Вектор пикселей выбранного изображения для очистки.
	@return Очищенный массив.
	*/
	vector<vector<Pixel>> clear_selected_image(vector<vector<Pixel>> image_vec);

	/**
	Вычисление невязки для исходного и восстановленного сигнала.

	@return Невязка.
	*/
	double calculate_difference();

	/**
	Вычислений модулей каждой точки в переданном изображении.
	
	@param data: Изображение.
	@return None
	*/
	void calculate_modules(vector<vector<Pixel>>& data);
};