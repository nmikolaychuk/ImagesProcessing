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
	double x;
	double y;
	Complex value;
	int color_value;

	/**
	Конструктор по умолчанию.
	*/
	Pixel();

	/**
	Конструктор с тремя параметрами.

	@param in_x: Координата "x" пикселя.
	@param in_y: Координата "y" пикселя.
	@param in_value: Значение пикселя.
	*/
	Pixel(double in_x, double in_y, double in_value);

	/**
	Перевод значения пикселя в цвет.

	@param min_value: Минимальное значение пикселя в изображении.
	@param max_value: Максимальное значение пикселя в изображении.
	@return Значение цвета [0-255].
	*/
	int get_color_from_value(double min_value, double max_value);

	/**
	Вычисление модуля комплексного числа.

	@return Модуль числа.
	*/
	double calc_module_of_cmplx();
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

	/**
	Конструктор по умолчанию.
	*/
	DrawSettings();

	/**
	Конструктор с инициализацией параметров.
	*/
	DrawSettings(CWnd* pic_wnd, CDC* pic_dc, CRect pic, double n_xmax, double n_ymax);

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

public:
	int image_width;
	int image_height;

	vector<Pixel> input_image;
	vector<Pixel> noise_image;
	vector<Pixel> spectrum_image;
	vector<Pixel> filtered_image;
	DrawSettings ds_input;
	DrawSettings ds_noise;

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
	Image(bool is_model, int width, int height, DrawSettings ds_in, DrawSettings ds_noi);

	/**
	Обновление состояния (параметров) объекта.

	@param is_model: Сгенерировать модель в качестве исходного изображения, если True.
		Загрузить существующее изображение в качестве исходного, если False.
	@param ds: Параметры рисования.
	@param width: Ширина изображения.
	@param height: Высота изображения.
	@return None
	*/
	void update_image_condition(bool is_model, int width, int height);

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

	@return Спектр изображения.
	*/
	vector<Pixel> get_spectrum_image();

	/**
	Фильтрация изображения.

	@return Отфильтрованное изображение.
	*/
	vector<Pixel> get_filtered_image();

	/**
	Доопределение исходного изображения до размера степени двойки.

	@return Доопределенное изображение.
	*/
	vector<Pixel> get_addition_input_image();

	/**
	Отрисовка изображения.
	
	@return None
	*/
	void draw_image(vector<Pixel> image, DrawSettings ds);

	/**
	Вычисление цвета по значениям изображения.

	@return Изображение с вычесленными цветами.
	*/
	vector<Pixel> calculate_color_of_image(vector<Pixel> image_vec);

	/**
	Очистка выбранного изображения.

	@param image_vec: Вектор пикселей выбранного изображения для очистки.
	@return Очищенный массив.
	*/
	vector<Pixel> clear_selected_image(vector<Pixel> image_vec);
};