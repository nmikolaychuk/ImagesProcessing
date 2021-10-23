#pragma once


/**
Определение точки изображения.
Имеются публичные поля:
x - координата x пикселя;
y - координата y пикселя;
value - значение пикселя (мировое, double);
color_value - значение пикселя (экранное, цвет).
*/
class Pixel {
public:
	double x;
	double y;
	double value;
	int color_value = 0;

	// Конструктор по умолчанию.
	Pixel() {
		x = 0.0;
		y = 0.0;
		value = 0.0;
	}

	/**
	Конструктор с тремя параметрами.

	@param in_x: Координата "x" пикселя.
	@param in_y: Координата "y" пикселя.
	@param in_value: Значение пикселя.
	*/
	Pixel(double in_x, double in_y, double in_value) {
		x = in_x;
		y = in_y;
		value = in_value;
	}

	/**
	Перевод значения пикселя в цвет.

	@param value: Значение пикселя.
	@param min_value: Минимальное значение пикселя в изображении.
	@param max_value: Максимальное значение пикселя в изображении.
	@return Цвет пикселя [0 - 255].
	*/
	int get_color_from_value(double value, double min_value, double max_value) {
		// Диапазон значений сигнала на каждый цвет.
		double color_range = (max_value - min_value) / 256.;
		
		for (int i = 0; i <= 255; i++) {
			double limit_value = color_range * i + color_range;
			if (value <= limit_value) return i;
		}
	}
};