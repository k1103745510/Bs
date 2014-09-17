#pragma once

class Math
{
public:
	Math(void);
	~Math(void);

	char		IntToCharX(int i){ return (i + 'A'); }
	char		IntToCharY(int i){ return (i + '1'); }
	int			CharToIntX(char i){ return (i - 'A'); }
	int			CharToIntY(char i){ return (i - '1'); }
	HitResult	ShortToHitRes(short i);

	void		SetColor(int color , int bgcolor);
	//0 ∫Œ≈Õ¿”
};

