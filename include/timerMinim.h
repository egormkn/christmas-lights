#pragma once

#ifndef TIMERMINIM_H
#define TIMERMINIM_H

class timerMinim
{
  public:
    timerMinim(uint32_t interval);				// объявление таймера с указанием интервала
    void setInterval(uint32_t interval);	// установка интервала работы таймера
    boolean isReady();						// возвращает true, когда пришло время. Сбрасывается в false сам (AUTO) или вручную (MANUAL)
    void reset();							// ручной сброс таймера на установленный интервал

  private:
    uint32_t _timer = 0;
    uint32_t _interval = 0;
};

#endif // TIMERMINIM_H
