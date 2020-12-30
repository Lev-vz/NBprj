-------------------------------------------------------------------------------
-- Функция сохраняет на диск список событий + указанный список переменных.
-- В переменной arg передаётся:
-- [1] - перечень сигнальных сигналов;
-- [2] - префикс имени файла;
-- [3] - глубина запроса данных [s];
-- [4] - интервал запроса данных [s];
-- [5] - перечень имён сохраняемых сигналов.
-------------------------------------------------------------------------------
local function dump(arg)
	local sigSignals = arg[1];
	local prefix = arg[2];
	local depth = arg[3];
	local interval = arg[4];
	local dataSignals = arg[5];
	-- Сбрасываем активные сигнальные сигналы.
	local dt_name = os.time();
	os.sleep(300);
	for _, name in ipairs(sigSignals) do
		if Core[name]==true then
			Core[name]=false;
		end
	end
	-- Получаем текущее время.
	local dt = os.time();
	-- Формируем имя файла.
	local t = os.date("*t", dt);
	local fileName = prefix.."_EVENTS"..os.date("_%Y%m%d_%H%M%S.log", dt_name);
	-- Открываем файл.
	local file = io.open(fileName, "w+");
	Core.addLogMsg("Оpen file Event");
	if file==nil then return end
	-- Запрашиваем список событий от текущего времени и depth секунд назад.
	local events = Core.getEvents(dt-depth, dt);
	
    -- Сохраняем события в файл.
	file:write("------------------------------------------------------------\n");
	file:write("Архив по событию\n");	
	file:write("------------------------------------------------------------\n");
	for _,event in ipairs(events) do
		local date = os.date("%Y.%m.%d %H:%M:%S.%3N", event.dt);
		local msg = string.format("%s\t%s\n", date, event.msg);
		file:write(msg);
	end
	file:close();
	file = nil;
	Core.addLogMsg("Close file Event");


	fileName = prefix.."_TREND"..os.date("_%Y%m%d_%H%M%S.log", dt_name);
	file = io.open(fileName, "w+");	
	Core.addLogMsg("Оpen file Trend");
	if file==nil then return end
	-- Сохраняем значения сигналов в файл.
	file:write("------------------------------------------------------------\n");
	file:write("Аварийный архив сигналов \n");		
	local msg = "Date&time\t";
	for _, name in ipairs(dataSignals) do
		msg = msg .. name.."\t";
	end
	file:write("------------------------------------------------------------\n");
	file:write(msg.."\n");
	for t = dt - depth, dt, interval do
		msg = os.date("%Y.%m.%d %H:%M:%S.%3N", t);
		for k, name in ipairs(dataSignals) do
			-- Запрашиваем из архива значение сигнала.
			local arch = Core(t)[name];
			if type(arch) ~= "table" then
				Core.addLogMsg("Signal "..name.." not found");
				dataSignals[k]= nil
			else
				if arch.res==0 then
					msg = msg.."\t"..tostring(arch.val);
				else
					msg = msg.."\t";
				end
			end
		end
		file:write(msg.."\n");
	end
	file:close();
	Core.addLogMsg("Close file Trend");
	file = nil;
end

MassAlarmValues = {
"ASArchive.Stop" -- Режим АО
,"ASArchive.Start" -- Запуск
,"Trend_AI.Tv_KGT"	--Температура под кожухом Д
,"Trend_AI.Tg_in_KND_1"	--Температура 1 перед камерой сгорания (за КНД)
,"Trend_AI.Tg_in_KND_2"	--Температура 2 перед камерой сгорания (за КНД)
,"Trend_AI.Tg_out_TND_1"	--Температура 1 на выходе ТНД
,"Trend_AI.Tg_out_TND_2"	--Температура 2 на выходе ТНД
,"Trend_AI.Tg_out_TND_3"	--Температура 3 на выходе ТНД
,"Trend_AI.Tg_out_TND_4"	--Температура 4 на выходе ТНД
,"Trend_AI.Tg_out_TND_5"	--Температура 5 на выходе ТНД
,"Trend_AI.Tg_out_TND_6"	--Температура 6 на выходе ТНД
,"Trend_AI.Tg_out_TND_7"	--Температура 7 на выходе ТНД
,"Trend_AI.Tg_out_TND_8"	--Температура 8 на выходе ТНД
,"Trend_AI.Tg_out_TND_9"	--Температура 9 на выходе ТНД
,"Trend_AI.Tg_out_TND_10"	--Температура 10 на выходе ТНД
,"Trend_AI.Tg_out_TND_11"	--Температура 11 на выходе ТНД
,"Trend_AI.Tg_out_TND_12"	--Температура 12 на выходе ТНД
,"Trend_AI.Tg_out_TND_13"	--Температура 13 на выходе ТНД
,"Trend_AI.Tg_out_TND_14"	--Температура 14 на выходе ТНД
,"Trend_AI.Tg_out_TND_15"	--Температура 15 на выходе ТНД
,"Trend_AI.Tg_out_TND_16"	--Температура 16 на выходе ТНД
,"Trend_AI.Tg_in_TND_1"	--Температура 1 за ТНД (т.17)
,"Trend_AI.Tg_in_TND_2"	--Температура 2 за ТНД (т.18)
,"Trend_AI.Tm_out_Per"	--Температура масла на сливе из переходника
,"Trend_AI.Tm_out_KVD"	--Температура масла на сливе из заднего корпуса КВД
,"Trend_AI.Tm_out_TND"	--Температура масла на сливе из опорного венца ТНД
,"Trend_AI.Tm_out_N"	--Температура масла на сливе из опорного венца ТН
,"Trend_AI.Tm_in_MBN"	--Температура масла в МБН
,"Trend_AI.Tm_in_MBD"	--Температура масла в МБД
,"Trend_AI.Tm_in_D"	--Температура масла на входе Д
,"Trend_AI.T_POD_1"	--Температура 1 подшипников промопоры
,"Trend_AI.T_POD_2"	--Температура 2 подшипников промопоры
,"Trend_AI.Tg_in_N"	--Температура газа на входе Н
,"Trend_AI.Tg_out_N"	--Температура газа на выходе Н
,"Trend_AI.T_PON"	--Температура переднего опорного подшипника Н
,"Trend_AI.Tp_N_opora"	--Температура опорно-упорного подшипника Н (опорная часть)
,"Trend_AI.Tp_N_work"	--Температура опорно-упорного подшипника Н (рабочая колодка)
,"Trend_AI.Tp_N_ust"	--Температура опорно-упорного подшипника Н (установочная колодка)
,"Trend_AI.Tms_N_AVO"	--Температура масла смазки нагнетателя после АВО
,"Trend_AI.T_atm"	--Температура атмосферного воздуха
,"Trend_AI.Tv_in_D"	--Температура воздуха на входе в двигатель
,"Trend_AI.T_H20_in_Ut"	--Температура воды на входе утилизатора
,"Trend_AI.T_H20_out_Ut"	--Температура воды на выходе утилизатора
,"Trend_AI.Ttg_in_BTA"	--Температура топливного газа на входе в БТА
,"Trend_AI.Razr_out_D"	--Разрежение воздуха на входе в двигатель
,"Trend_AI.Pm_D"	--Давление масла на входе в двигатель (после фильтра)
,"Trend_AI.Pm_N"	--Давление масла смазки Н
,"Trend_AI.dP_mg"	--Перепад давлений масло-газ
,"Trend_AI.Ptg_in_BTA"	--Давление топливного газа на входе БТА
,"Trend_AI.dPtg_in_filtr"	--Перепад давлений топливного газа на фильтре
,"Trend_AI.Pg_in_N"	--Давление газа на входе Н
,"Trend_AI.Ptg_zamer"	--Давление топливного газа на замерной нитке
,"Trend_AI.dPtg_in_diaf"	--Перепад давлений топливного газа на диафрагме
,"Trend_AI.Pg_out_N"	--Давление газа на выходе Н
,"Trend_AI.dP_conf"	--Перепад давлений на конфузоре
,"Trend_AI.P_atm"	--Давление атмосферного воздуха
,"Trend_AI.dPg_fors"	--Перепад давлений газа на форсунках
,"Trend_AI.Pv_out_KVD"	--Давление воздуха за КВД
,"Trend_AI.dPm_in_filtr_N"	--Перепад давлений масла на фильтре Н (ТП-22)
,"Trend_AI.Lm_MBN"	--Уровень в МБН
,"Trend_AI.Vibr_OP_N"	--Вибросмещение ОП нагнетателя
,"Trend_AI.Vibr_OUP_N"	--Вибросмещение ОУП нагнетателя
,"Trend_AI.Vibr_KVD"	--Вибрация корпуса КВД
,"Trend_AI.Vibr_KND"	--Вибрация корпуса КНД
,"Trend_AI.Vibr_N"	--Вибрация корпуса ТН
,"Trend_AI.Pos_TRK"	--Положение топливно-регулирующего клапана (ДУС)
,"Trend_AI.KCU_Upr_TRK"	--КЦУ. Управление топливно-регулирующим клапаном 
,"Trend_AI.U_osn_RKBU"	--Напряжение основного питания РК БУ
,"Trend_AI.U_res_RKBU"	--Напряжение резервного питания РК БУ
,"Trend_AI.T_PLC"	--Температура контроллера
,"Trend_AI.N_KND_1"	--Частота вращения КНД канал 1
,"Trend_AI.N_KVD_1"	--Частота вращения КВД канал 1
,"Trend_AI.N_TN_1"	--Частота вращения ротора ТН канал 1
,"Trend_AI.N_KND_2"	--Частота вращения КНД канал 2
,"Trend_AI.N_KVD_2"	--Частота вращения КВД канал 2
,"Trend_AI.N_TN_2"	--Частота вращения ротора ТН канал 2
,"Trend_DI.Kr1_ON"	--Кран № 1 открыт
,"Trend_DI.KCD_Kr1_ON"	--КЦД. Кран № 1 открыт
,"Trend_DI.Kr1_OF"	--Кран № 1 закрыт
,"Trend_DI.KCD_Kr1_OF"	--КЦД. Кран № 1 закрыт
,"Trend_DI.Kr2_ON"	--Кран № 2 открыт
,"Trend_DI.KCD_Kr2_ON"	--КЦД. Кран № 2 открыт
,"Trend_DI.Kr2_OF"	--Кран № 2 закрыт
,"Trend_DI.KCD_Kr2_OF"	--КЦД. Кран № 2 закрыт
,"Trend_DI.Kr4_ON"	--Кран № 4 открыт
,"Trend_DI.KCD_Kr4_ON"	--КЦД. Кран № 4 открыт
,"Trend_DI.Kr4_OF"	--Кран № 4 закрыт
,"Trend_DI.KCD_Kr4_OF"	--КЦД. Кран № 4 закрыт
,"Trend_DI.Kr5_ON"	--Кран № 5 открыт
,"Trend_DI.KCD_Kr5_ON"	--КЦД. Кран № 5 открыт
,"Trend_DI.Kr5_OF"	--Кран № 5 закрыт
,"Trend_DI.KCD_Kr5_OF"	--КЦД. Кран № 5 закрыт
,"Trend_DI.Kr6_ON"	--Кран № 6 открыт
,"Trend_DI.KCD_Kr6_ON"	--КЦД. Кран № 6 открыт
,"Trend_DI.Kr6_OF"	--Кран № 6 закрыт
,"Trend_DI.KCD_Kr6_OF"	--КЦД. Кран № 6 закрыт
,"Trend_DI.Kr9_ON"	--Кран № 9 открыт
,"Trend_DI.KCD_Kr9_ON"	--КЦД. Кран № 9 открыт
,"Trend_DI.Kr9_OF"	--Кран № 9 закрыт
,"Trend_DI.KCD_Kr9_OF"	--КЦД. Кран № 9 закрыт
,"Trend_DI.Kr12_ON"	--Кран № 12 открыт
,"Trend_DI.KCD_Kr12_ON"	--КЦД. Кран № 12 открыт
,"Trend_DI.Kr12_OF"	--Кран № 12 закрыт
,"Trend_DI.KCD_Kr12_OF"	--КЦД. Кран № 12 закрыт
,"Trend_DI.dPm_in_D"	--Перепад давлений масла на фильтре на входе в Д больше 0.1 МПа
,"Trend_DI.dPm_in_BTA"	--Перепад давлений масла на фильтре на входе в БТА больше 0.1 МПа
,"Trend_DI.dPmg"	--Перепад давлений масло-газ меньше 0.03 Мпа
,"Trend_DI.KCD_dPmg"	--КЦД. Перепад давлений масло-газ меньше 0.03 Мпа
,"Trend_DI.Sdvig_N_ahead"	--Осевой сдвиг ротора нагнетателя вперед (к турбине)
,"Trend_DI.KCD_Sdvig_N_ahead"	--КЦД. Осевой сдвиг ротора нагнетателя вперед (к турбине)
,"Trend_DI.Sdvig_N_back"	--Осевой сдвиг ротора нагнетателя назад (от турбины)
,"Trend_DI.KCD_Sdvig_N_back"	--КЦД. Осевой сдвиг ротора нагнетателя назад (от турбины)
,"Trend_DI.Pomp_N"	--Помпаж нагнетателя
,"Trend_DI.KCD_Pomp_N"	--КЦД. Помпаж нагнетателя
,"Trend_DI.Pm_in_D_alarm_low"	--Давление масла на входе в Д аварийно низкое меньше 0.1 МПа
,"Trend_DI.KCD_Pm_in_D_alarm_low"	--КЦД. Давление масла на входе в Д аварийно низкое меньше 0.1 Мпа
,"Trend_DI.Pm_in_D_002"	--Давление масла на входе в Д больше 0.02 МПа
,"Trend_DI.KCD_Pm_in_D_003"	--КЦД. Давление масла на входе в Д больше 0.02 Мпа
,"Trend_DI.Pm_in_D_low002"	--Давление масла на входе в Д низкое меньше 0.2 МПа
,"Trend_DI.dPm_MOB_005"	--Перепад давлений масла на МОБе больше 0.05 МПа
,"Trend_DI.Pm_MNOD_002"	--Давление масла за эл. МНОД больше 0,02 МПа
,"Trend_DI.KCD_Pm_MNOD_003"	--КЦД. Давление масла за эл. МНОД больше 0,02 Мпа
,"Trend_DI.Pm_MNOD_004"	--Давление масла за нав. МНОД больше 0.04 МПа
,"Trend_DI.Pm_till_MNU"	--Давление масла в трубопроводе до МНУ
,"Trend_DI.Pg_in_P_N"	--Давление газа в полости Н
,"Trend_DI.Pm_POD_N"	--Давление масла на смазку подшипников Н
,"Trend_DI.dPg_in_Kr1"	--Перепад давлений газа на кране N1
,"Trend_DI.Pm_N_RD1"	--Давление масла на входе в систему смазки Н РД1
,"Trend_DI.Pv_MB_N_002"	--Давление воздуха в маслобаке Д больше 0.02 МПа
,"Trend_DI.Pm_upl_low"	--Давление масла уплотнения низкое
,"Trend_DI.U_PMNS"	--Наличие напряжения в цепях управления ПМНС
,"Trend_DI.U_MNU_1"	--Наличие напряжения в цепях управления МНУ I
,"Trend_DI.U_MNU_2"	--Наличие напряжения в цепях управления МНУ II
,"Trend_DI.Lm_MBN_up"	--Уровень масла в МБН верхний
,"Trend_DI.Lm_MBN_down"	--Уровень масла в МБН нижний
,"Trend_DI.Lm_MBD_up"	--Уровень масла в МБД верхний
,"Trend_DI.Lm_MBD_alarm"	--Уровень масла в МБД аварийный
,"Trend_DI.KCD_Lm_MBD_alarm"	--КЦД. Уровень масла в МБД аварийный
,"Trend_DI.Lm_MBD_down"	--Уровень масла в МБД нижний
,"Trend_DI.SK_ON"	--Стоп-кран открыт
,"Trend_DI.SK_OF"	--Стоп-кран закрыт
,"Trend_DI.Pg_in_BTA_16"	--Давление газа на входе в БТА больше 1.6 МПа
,"Trend_DI.Chips_KP"	--Наличие стружки на сливе из КП
,"Trend_DI.Chips_Per"	--Наличие стружки на сливе из переходника
,"Trend_DI.Chips_KVD"	--Наличие стружки на сливе из ЗК КВД
,"Trend_DI.Chips_TND"	--Наличие стружки на сливе из ОВ ТНД Д
,"Trend_DI.Chips_TN"	--Наличие стружки на сливе из ОВ ТН Д
,"Trend_DI.PMNS_ON"	--ПМНС Н включен
,"Trend_DI.PMNS_OF"	--ПМНС Н отключен
,"Trend_DI.MNU_1_ON"	--МНУ (1 секция) включен
,"Trend_DI.MNU_1_OF"	--МНУ (1 секция) отключен
,"Trend_DI.MNU_2_ON"	--МНУ (2 секция) включен
,"Trend_DI.MNU_2_OF"	--МНУ (2 секция) отключен
,"Trend_DI.MNND_ON"	--МННД включен
,"Trend_DI.MNND_OF"	--МННД отключен
,"Trend_DI.MNOD_ON"	--МНОД включен
,"Trend_DI.MNOD_OF"	--МНОД отключен
,"Trend_DI.MNOB_ON"	--МНОБ Д включен
,"Trend_DI.MNOB_OF"	--МНОБ Д отключен
,"Trend_DI.VOD_1_ON"	--ВОД 1 включен
,"Trend_DI.VOD_2_ON"	--ВОД 2 включен
,"Trend_DI.VMON_1_ON"	--ВМОН1 включен
,"Trend_DI.VMON_4_ON"	--ВМОН4 включен
,"Trend_DI.VMOD_2_ON"	--ВМОД2 включен
,"Trend_DI.VMOD_3_ON"	--ВМОД3 включен
,"Trend_DI.VMOD_5_ON"	--ВМОД5 включен
,"Trend_DI.VMOD_6_ON"	--ВМОД6 включен
,"Trend_DI.KU_and_ON"	--КУ в КК1 и КК2 включены
,"Trend_DI.KU_or_ON"	--КУ в КК1 или КК2 включен
,"Trend_DI.K1_KK1_1_ON"	--К1 1ск. (РК1) в КК1 включен
,"Trend_DI.K2_KK1_2_ON"	--К2 2ск. (РК2) в КК1 включен
,"Trend_DI.KU_KK1_ON"	--КУ в КК1 включен
,"Trend_DI.K1_KK2_1_ON"	--К1 1ск. (РК1) в КК2 включен
,"Trend_DI.K2_KK2_2_ON"	--К2 2ск. (РК2) в КК2 включен
,"Trend_DI.KU_KK2_ON"	--КУ в КК2 включен
,"Trend_DI.U380_KK"	--Наличие напряжения ~380 В в КК
,"Trend_DI.TEN1_MBN_ON"	--ТЭН1 в МБН включен
,"Trend_DI.TEN2_MBN_ON"	--ТЭН2 в МБН включен
,"Trend_DI.TEN3_MBN_ON"	--ТЭН3 в МБН включен
,"Trend_DI.TEN4_MBN_ON"	--ТЭН4 в МБН включен
,"Trend_DI.TEN1_N_ON"	--ТЭН1 трубопроводов Н включен
,"Trend_DI.TEN2_N_ON"	--ТЭН2 трубопроводов Н включен
,"Trend_DI.TEN3_N_ON"	--ТЭН3 трубопроводов Н включен
,"Trend_DI.TEN4_N_ON"	--ТЭН4 трубопроводов Н включен
,"Trend_DI.TEN_MBD_ON"	--ТЭН МБД включен
,"Trend_DI.ORT_check"	--Контроль ОРТ
,"Trend_DI.ORT_alarm"	--Авария ОРТ
,"Trend_DI.KCD_ORT_alarm"	--КЦД. Авария ОРТ
,"Trend_DI.Pos_m10_VNA"	--ВНА в положении -10
,"Trend_DI.Pos_p15_VNA"	--ВНА в положении +15
,"Trend_DI.Gas_N"	--Загазованность галереи Н больше 1 %
,"Trend_DI.KCD_Gas_N"	--КЦД. Загазованность галереи Н больше 1 %
,"Trend_DI.ICE_VOU"	--Обледенение ВОУ (СОУ-1)
,"Trend_DI.Fire"	--Пожар в МБД, МБН, МФ (порошок)
,"Trend_DI.KCD_Fire"	--КЦД. Пожар в МБД, МБН, МФ (порошок)
,"Trend_DI.pgrP_brk"	--Сист.порош.пожаротуш.неисправ.
,"Trend_DI.AO_btn"	--Аварийный останов (АО) от кнопки в машзале
,"Trend_DI.KCD_AO_btn"	--КЦД. Аварийный останов (АО) от кнопки в машзале
,"Trend_DI.AO_SAU"	--Аварийный останов (АО) от САУ КЦ
,"Trend_DI.KCD_AO_SAU"	--КЦД. Аварийный останов (АО) от САУ КЦ
,"Trend_DI.Door_SAU_open"	--Двери блок-бокса САУ ГПА открыты
,"Trend_DI.KCU_Kr1_ON"	--КЦУ. Кран №1 открыть
,"Trend_DI.KCU_Kr1_OF"	--КЦУ. Кран №1 закрыть
,"Trend_DI.KCU_Kr1_oil"	--КЦУ. Кран № 1 смазка
,"Trend_DI.KCU_Kr2_ON"	--КЦУ. Кран № 2 открыть
,"Trend_DI.KCU_Kr2_OF"	--КЦУ. Кран № 2 закрыть
,"Trend_DI.KCU_Kr2_oil"	--КЦУ. Кран № 2 смазка
,"Trend_DI.KCU_Kr4_ON"	--КЦУ. Кран № 4 открыть
,"Trend_DI.KCU_Kr4_OF"	--КЦУ. Кран № 4 закрыть
,"Trend_DI.KCU_Kr5_ON"	--КЦУ. Кран № 5 открыть
,"Trend_DI.KCU_Kr5_OF"	--КЦУ. Кран № 5 закрыть
,"Trend_DI.KCU_Kr6_ON"	--КЦУ. ран № 6 открыть
,"Trend_DI.KCU_Kr6_OF"	--КЦУ. Кран № 6 закрыть
,"Trend_DI.KCU_Kr9_ON"	--КЦУ. Кран № 9 открыть
,"Trend_DI.KCU_Kr9_OF"	--КЦУ. Кран № 9 закрыть
,"Trend_DI.KCU_Kr12_ON"	--КЦУ. Кран № 12 открыть
,"Trend_DI.KCU_Kr12_OF"	--КЦУ. Кран № 12 закрыть
,"Trend_DI.KCU_PMNS_OM"	--КЦУ. ПМНС Н включить
,"Trend_DI.KCU_MNU_1_ON"	--КЦУ. МНУ (1 секция) включить
,"Trend_DI.KCU_MNU_2_ON"	--КЦУ. МНУ (2 секция) включить
,"Trend_DI.KCU_MNND_ON"	--КЦУ. МННД включить
,"Trend_DI.KCU_MNOD_ON"	--КЦУ. МНОД включить
,"Trend_DI.KCU_MNOB_D_ON"	--КЦУ. МНОБ Д включить
,"Trend_DI.KCU_VOD_1_ON"	--КЦУ. ВОД1 включить
,"Trend_DI.KCU_VOD_2_ON"	--КЦУ. ВОД2 включить
,"Trend_DI.Check_U_AC220"	--Контроль основного питания РК БУ ~220 В
,"Trend_DI.Check_U_DC220"	--Контроль резервного питания РК БУ =220 В
,"Trend_DI.Check_UZIP"	--Контроль исправности разрядников
,"Trend_DI.QF_ON"	--Автоматы питания включены
,"Trend_DI.Check_U_DC220_BEO_osn"	--Контроль основного питания БЭАО ~220 В
,"Trend_DI.Check_U_DC220_BEO_rez"	--Контроль резервного питания БЭАО =220 В
,"Trend_DI.Check_U_AI_Tp"	--Контроль питания терминальных панелей AI
,"Trend_DI.Check_U_DI_Tp"	--Контроль питания терминальных панелей DI
,"Trend_DI.Check_U_D0_Tp"	--Контроль питания терминальных панелей DO
,"Trend_DI.Check_U_DC24_PLC_osn"	--Исправность основного ИП =24В ПЛК
,"Trend_DI.Check_U_DC24_PLC_rez"	--Исправность резервного ИП =24В ПЛК
,"Trend_DI.Check_U_DC24_VU_osn"	--Исправность основного ИП =24В внутренних устройств
,"Trend_DI.Check_U_DC24_VU_rez"	--Исправность резервного ИП =24В внутренних устройств
,"Trend_DI.Check_U_DC24_AI_osn"	--Исправность основного ИП =24В аналоговых входов
,"Trend_DI.Check_U_DC24_AI_rez"	--Исправность резервного ИП =24В аналоговых входов
,"Trend_DI.Check_U_DC24_DI_osn"	--Исправность основного ИП =24В DI, (внеш.)
,"Trend_DI.Check_U_DC24_DI_rez"	--Исправность резервного ИП =24В DI, (внеш.)
,"Trend_DI.Check_U_DC24_VU_1_osn"	--Исправность основного ИП =24В внешних устройств
,"Trend_DI.Check_U_DC24_VU_1_rez"	--Исправность резервного ИП =24В внешних устройств
,"Trend_DI.Check_U_DC24_BEO_osn"	--Исправность основного ИП =24В БЭО, РПКУ
,"Trend_DI.Check_U_DC24_BEO_rez"	--Исправность резервного ИП =24В БЭО , РПКУ
,"Trend_DI.Check_U_CO1_plus"	--Контроль питания +15 В СО-1
,"Trend_DI.Check_U_CO1_minus"	--Контроль питания -15 В СО-1
,"Trend_DI.But_BEO_ON"	--Кнопка «ЭАО» нажата
,"Trend_DI.KCD_But_BEO_ON"	--КЦД. Кнопка «ЭАО» нажата
,"Trend_DI.But_AO_ON"	--Кнопка «АО ГПА» нажата
,"Trend_DI.KCD_But_AO_ON"	--КЦД. Кнопка «АО ГПА» нажата
,"Trend_DI.But_NO_ON"	--Кнопка «НО ГПА» нажата
,"Trend_DI.BEO_ready"	--Готовность БЭО
,"Trend_DI.But_Kvit_ON"	--Кнопка «Квитирование» нажата
,"Trend_DI.BEO_work"	--БЭО работает
,"Trend_DI.Check_U_osn_BUSHD220"	--Контроль основного питания БУШД ~220 В
,"Trend_DI.Check_U_osn_BUSHD24"	--Контроль резервного питания БУШД =24 В
,"Trend_DI.Chec_Ukr_110"	--Контроль питания кранов =110 В
,"Trend_DI.Check_U_PRU24"	--Контроль питания ПРУ =24 В
,"Trend_DI.Check_Uvibr24"	--Контроль питания датчиков уровня и вибрации =24 В
,"Trend_DO.Kr1_ON"	--Кран № 1 открыть
,"Trend_DO.Kr1_OF"	--Кран № 1 закрыть
,"Trend_DO.Kr1_oil"	--Кран № 1 смазка
,"Trend_DO.Kr2_ON"	--Кран № 2 открыть
,"Trend_DO.Kr2_OF"	--Кран № 2 закрыть
,"Trend_DO.Kr2_oil"	--Кран № 2 смазка
,"Trend_DO.Kr4_ON"	--Кран № 4 открыть
,"Trend_DO.Kr4_OF"	--Кран № 4 закрыть
,"Trend_DO.Kr5_ON"	--Кран № 5 открыть
,"Trend_DO.Kr5_OF"	--Кран № 5 закрыть
,"Trend_DO.Kr6_ON"	--Кран № 6 открыть
,"Trend_DO.Kr6_OF"	--Кран № 6 закрыть
,"Trend_DO.Kr9_ON"	--Кран № 9 открыть 
,"Trend_DO.Kr9_OF"	--Кран № 9 закрыть
,"Trend_DO.Kr12_ON"	--Кран № 12 открыть
,"Trend_DO.Kr12_OF"	--Кран № 12 закрыть
,"Trend_DO.PMNS_ON"	--ПМНС Н включить
,"Trend_DO.MNU_1_ON"	--МНУ (1 секция) включить
,"Trend_DO.MNU_2_ON"	--МНУ (2 секция) включить
,"Trend_DO.MNND_ON"	--МННД включить
,"Trend_DO.MNOD_ON"	--МНОД включить
,"Trend_DO.MNOB_ON"	--МНОБ Д включить
,"Trend_DO.VOD_1_ON"	--ВОД 1 включить
,"Trend_DO.VOD_2_ON"	--ВОД 2 включить
,"Trend_DO.VMON_1_ON"	--ВМОН1 включить
,"Trend_DO.VMOD_2_ON"	--ВМОД2 включить
,"Trend_DO.VMOD_3_ON"	--ВМОД3 включить
,"Trend_DO.VMON_4_ON"	--ВМОН4 включить
,"Trend_DO.VMOD_5_ON"	--ВМОД5 включить
,"Trend_DO.VMOD_6_ON"	--ВМОД6 включить
,"Trend_DO.TEN_MBD_ON"	--ТЭН МБД включить
,"Trend_DO.TEN1_MBN_ON"	--ТЭН1 в МБН включить
,"Trend_DO.TEN2_MBN_ON"	--ТЭН2 в МБН включить
,"Trend_DO.TEN3_MBN_ON"	--ТЭН3 в МБН включить
,"Trend_DO.TEN4_MBN_ON"	--ТЭН4 в МБН включить
,"Trend_DO.TEN1_N_ON"	--ТЭН1 трубопроводов Н включить
,"Trend_DO.TEN2_N_ON"	--ТЭН2 трубопроводов Н включить
,"Trend_DO.TEN3_N_ON"	--ТЭН3 трубопроводов Н включить
,"Trend_DO.TEN4_N_ON"	--ТЭН4 трубопроводов Н включить
,"Trend_DO.Alarm_sound_ON"	--Звуковую сигнализацию включить
,"Trend_DO.Light_ON"	--Освещение включить
,"Trend_DO.SK_ON"	--Питание на клапан СК включить
,"Trend_DO.KPG_ON"	--КПГ открыть (отключить)
,"Trend_DO.U_KNO_ON"	--Питание на КНО включить
,"Trend_DO.U_KNO_udrj12_ON"	--Питание на КНО включить (удержание12 В)
,"Trend_DO.U_KAO_ON"	--Питание на КАО включить
,"Trend_DO.BP_SPV_ON"	--БП плазменных воспламенителей СПВ включить
,"Trend_DO.KPR1_ON"	--КПР1 открыть
,"Trend_DO.KPR2_ON"	--КПР2 открыть
,"Trend_DO.ORT_ON"	--ОРТ включить
,"Trend_DO.Check_mex_perm_ON"	--Контроль механизмов с БС разрешить
,"Trend_DO.Starter_upr_ON"	--Реле управления стартерами включить
,"Trend_DO.Speed_ON"	--Реле переключения скоростей включить
,"Trend_DO.Chips_chek_alarm_ON"	--Контроль сигнализаторов стружки (контроль САУ)
,"Trend_DO.PLC_wor_ON"	--Исправность ПЛК
,"Trend_DO.Ind_PS_ON"	--Светозвуковая сигнализация "ПС"
,"Trend_DO.Ind_AS_ON"	--Светозвуковая сигнализация "АС" 
,"AnOut.Set_TRK"	--Управление иглой ДГ ДС-В-3МП

}


-- Формат функции регистрации обработчиков следующий.
-- Core.onExtChange( event_signals, event_function, event_function_arg)
-- event_signals - таблица с перечнем имён сигналов, при изменении которых вызывать обработчик.
-- event_function - функция-обработчик, с единственным аргументом.
-- event_function_arg - аргумент функции-обработчика (может быть любым типом (в том числе и таблицей).

-- Регистрируем обработчик события изменения сигналов.
Core.onExtChange(
				{"ASArchive.Stop" },		-- Перечень имён сигналов, при изменении которых выполнять вызов функции dump.
				dump,							-- Вызываемая функция
				{
					{},	-- Передаём в обработчик перечень имён сигналов, при изменении которых выполнился вызов функции, чтобы сбросить их.
					"AO",					    -- Префикс имени файла.
					900, 						-- Глубина запроса данных [s].
					1,							-- Интервал запроса данных [s];
					MassAlarmValues		-- Перечень имён сохраняемых сигналов.
				}
);

-- Регистрируем обработчик события изменения сигналов.
Core.onExtChange(
				{"ASArchive.Start" },		-- Перечень имён сигналов, при изменении которых выполнять вызов функции dump.
				dump,							-- Вызываемая функция
				{
					{},	-- Передаём в обработчик перечень имён сигналов, при изменении которых выполнился вызов функции, чтобы сбросить их.
					"Start",					    -- Префикс имени файла.
					900, 						-- Глубина запроса данных [s].
					1,							-- Интервал запроса данных [s];
					MassAlarmValues		-- Перечень имён сохраняемых сигналов.
				}
);


--[[
-- Регистрируем обработчик события изменения других сигналов.
Core.onExtChange(
				{"Critical_1", "Critical_2"},	-- Перечень имён сигналов, при изменении которых выполнять вызов функции dump.
				dump,							-- Вызываемая функция
				{
					{"Critical_1", "Critical_2"},-- Передаём в обработчик перечень имён сигналов, при изменении которых выполнился вызов функции, чтобы сбросить их.
					"Critical",					-- Префикс имени файла.
					100, 						-- Глубина запроса данных [s].
					1,							-- Интервал запроса данных [s];
					{"Real_3", "Real_4"}		-- Перечень имён сохраняемых сигналов.
				}
);
]]
-- Переходим к ожиданию событий.
Core.waitEvents();

