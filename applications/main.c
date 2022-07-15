Button key2(GET_PIN(E, 0), false, PIN_MODE_INPUT_PULLUP, 30);
    Button key3(GET_PIN(D, 6), false, PIN_MODE_INPUT_PULLUP, 30);

    CalcSpeed<double, 50> left("htm2", 50000);
    CalcSpeed<double, 50> left("htm2", 30000);
    //left.setInverse(true);
    CalcSpeed<double, 50> right("htm1", 50000);
    CalcSpeed<double, 50> right("htm4", 32000);

    double refl = 0;
    double refr = 0;
    Pid<double> pleft(refl, 800, 2, 1, 100000, -100000);
    Pid<double> pright(refr, 800, 2, 1, 100000, -100000);

    Pwm right_pwm("pwm4" ,100000, 1 ,2);
    Pwm right_pwm("pwm1" ,100000, 1 ,2);
    Pwm left_pwm("pwm3" ,100000, 1 ,2);
    //left_pwm.setInverse(true);

    double wheel_base = 0.21; //m
    double wheel_radius = 0.035; //m



    while (1)
    {
         static double speed[2] = {0.0, 0.0};
        rt_mq_recv(&mq, &speed, 2*sizeof(double), 0);
        if(abs(speed[0]) > 0 && abs(speed[1]) == 0){
            pleft.setRef(speed[0]*100);
            pright.setRef(speed[0]*100);
        }else if(abs(speed[1]) > 0 && abs(speed[0]) == 0){
            pleft.setRef(-speed[1]*100);
            pright.setRef(speed[1]*100);
        }else if(abs(speed[1]) == 0 && abs(speed[0]) == 0){
            pleft.setRef(0);
            pright.setRef(0);
        double left_speed = (60 * (2 * speed[0] - wheel_base * speed[1]) * 0.5f) / (2*3.14*wheel_radius);
        double right_speed = (60 * (2 * speed[0] + wheel_base * speed[1]) * 0.5f) / (2*3.14*wheel_radius);

        if(fabs(left_speed) > 200){
            left_speed = left_speed > 0 ? 200 : -200;
        }
        if(fabs(right_speed) > 200){
            right_speed = right_speed > 0 ? 200 : -200;
       }

        pleft.setRef(left_speed);
        pright.setRef(right_speed);

        /****************/
//        left_pwm.setCW(50000);
@@ -103,7 +109,7 @@ void speedThread(void* parameter)
        /********************/
        if(cnt++ >= 100){
            cnt = 0;
            //rt_kprintf("%f|%f|%f|%f\r\n", left.calc(), right.calc(), speed[0], speed[1]);
            rt_kprintf("%f|%f|%f|%f\r\n", left.calc(), right.calc(), left_speed, right_speed);
        }
        rt_thread_mdelay(1);
    }
	     if(_first){
            _first = false;
            _last_time = rt_tick_get();
            if(strcmp(_encode_name, "htm1") == 0){
                _last_cnt = getCounter(1);
            if(strcmp(_encode_name, "htm4") == 0){
                _last_cnt = getCounter(4);
            }else if(strcmp(_encode_name, "htm2") == 0){
                _last_cnt = getCounter(2);
            }
        }
        rt_uint32_t now_cnt;
        if(strcmp(_encode_name, "htm1") == 0){
            now_cnt = getCounter(1);
        rt_int16_t now_cnt;
        if(strcmp(_encode_name, "htm4") == 0){
            now_cnt = getCounter(4);
        }else if(strcmp(_encode_name, "htm2") == 0){
            now_cnt = getCounter(2);
        }
        rt_int32_t diff_cnt = (now_cnt - _last_cnt);
        rt_int16_t diff_cnt = (now_cnt - _last_cnt);
        rt_tick_t now_time = rt_tick_get();
        rt_tick_t diff_t = now_time - _last_time;
        diff_t = diff_t == 0 ? 1 : diff_t;
@@ -55,7 +55,7 @@ class CalcSpeed {
private:
    SlideWindow<T, N> _filter;
    char _encode_name[6];
    rt_uint32_t _last_cnt{0};
    rt_int16_t _last_cnt{0};
    rt_tick_t _last_time{0};
    TIM_HandleTypeDef _tim;
    rt_uint16_t _cpr{50000};
