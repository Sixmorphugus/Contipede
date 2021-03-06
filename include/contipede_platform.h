#pragma once

// very useful functions
// for the sake of organization everything in here starts with "cont_plat_"
void cont_plat_waitms(int numMS); // stops execution for a certain number of miliseconds

int cont_plat_get_frametime();
int cont_plat_speed_increment(double input);
int cont_plat_timeout_from_speed(double speed);

void cont_plat_beep(); // must be done inside "draw"