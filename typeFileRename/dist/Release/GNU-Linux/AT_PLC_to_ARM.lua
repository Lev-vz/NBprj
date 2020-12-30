-----------------------------------------------------------

-------------------------------------------------------------------------------------------
function GetMode()

local TagName_m = {
'us.AOss',	
					'us.AObs',	
					'us.NOss',	
					'us.NObs',	
					'us.Repair',	
					'us.HR'	,
				    'us.GR',
					'us.PZ',	
					'us.PZM',	
					'us.HP',	
					'us.OGR',	
					'us.KPK',	
					'us.APK',
					'us.APM',
					'us.K',	
					'us.M',	
					'us.K_M',	
					'us.M_K',	
					'us.Fault',
					'us.PBZ',
					'us.TP'}--,	
--					'us.Stop'	}
local TagComment_m = {}




for i=1, #TagName_m do
table.insert (TagComment_m, Core.getSignalComment(TagName_m[i]))
end


for j = 1 , #TagName_m do 
 if Core[""..TagName_m[j]..""] == true then Core["Mode_String"] = TagComment_m[j] end
end
 if Core["UnitState"] > 100 and Core["UnitState"] < 200 then Core["Mode_Color"] = 1  end
 if Core["UnitState"] > 300 then Core["Mode_Color"] = 2 end;
 if Core["UnitState"] > 200 and Core["UnitState"] < 300 then Core["Mode_Color"] = 3 end

end


-------------------------------------------------------------------------------------------

-------------------------------------------------------------------------------------------
function GetStatus()
--local ConString
local TagName_s = {
			'TS.GR_noReady',
			'TS.HR_noReady',
			'TS.HR_Ready',
			'TS.GR_Ready',
			'TS.HR_Ready_G',
			'TS.GR_Ready_G',
			'TS.Start_MD',
			'TS.Wait_blow_N',
			'TS.Stab_Pg_N',
			'TS.Start_TG',
			'TS.noG_inN',
			'TS.blowing_N',
			'TS.stop_Finish',
			'TS.Check_SAU',
			'TS.Ready_KPK',
			'TS.Ready_APK',
			'TS.Ready_APM',
			'TS.Ready_PBZ',
			'TS.Ready_HP',
			'TS.Ready_TP',
			'TS.Ready_PZM',
			'TS.StartGTD',
			'TS.IMIT_ON',
			'TS.wasGas_ON',
			'TS.filling_MN',
			'TS.Stab_MN',
			'TS.filling_MUN',
			'TS.Stab_MUN',
			'TS.Sound_ON',
			'TS.Check_PZM',
			'TS.Check_PZM_finish',
			'TS.HP_work',
			'TS.HP_finish',
			'TS.AOss_finish',
			'TS.AObs_finish',
			'TS.VOD_15m_OF',
			'TS.VOD_100s_OF',
			'TS.GNSD_False',
			'TS.GNU_False',
			'TS.Wait_blow_TG',
			'TS.GNSN_False',
			'TS.Check_PZ',
--			'TS.VOD_1_main',
--			'TS.VOD_2_main',
--			'TS.MNU_1_main',
--			'TS.MNU_2_main',
			'TS.is_flame',
			'TS.goHH',
			'TS.warm_HH',
			'TS.goNARR',
			'TS.warm_NARR',
			'TS.cooling_NARR',
			'TS.N_HH',
			'TS.CoolingGTD'
					}
local TagComment_s = {}
local ConString ={}

for i=1, #TagName_s do
table.insert(TagComment_s, Core.getSignalComment(TagName_s[i]))
end;



for j = 1 , #TagName_s  do
  if Core[""..TagName_s[j]..""] == true then  table.insert( ConString, TagComment_s[j]) end;
end;
Core["Status_String"] = table.concat(ConString, "\n") 


end;
--------------------------------------------------------------------------
function drum()
Core.STEP_NUM = 24
if 	Core.Calc_AI.Drum_AObs_Step>= 1 or 
	Core.Calc_AI.Drum_AOss_Step >= 1 or 
	Core.Calc_AI.Drum_APK_Step >= 1 or
	Core.Calc_AI.Drum_APM_Step >= 1 or 
	Core.Calc_AI.Drum_HP_Step>= 1 or 
	Core.Calc_AI.Drum_K_M_Step >= 1 or 
	Core.Calc_AI.Drum_M_K_Step >= 1 or
	Core.Calc_AI.Drum_KPK_Step >= 1 or 
	Core.Calc_AI.Drum_NObs_Step>= 1 or 
	Core.Calc_AI.Drum_NOss_Step >= 1 or 
	Core.Calc_AI.Drum_OGR_Step >= 1 or
	Core.Calc_AI.Drum_PZ_Step >= 1 or
	Core.Calc_AI.Drum_PZM_Step >= 1 or
	Core.Calc_AI.Drum_PBZ_Step >= 1 or
	Core.Calc_AI.Drum_TP_Step >= 1 
then

-----Режим АОбс
if Core.Calc_AI.Drum_AObs_Step >= 1  then
	for i= 0 , 23 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
	Core.STEP_STR[0] = "1.Останов зав." 					--1 шаг
    Core.STEP_STR[1] = "2.Деблокировка" 					--2 шаг
if Core.Calc_AI.Drum_AObs_Step == 1 then Core.STEP[0] = true else Core.STEP[0] = false end
if Core.Calc_AI.Drum_AObs_Step == 2 then Core.STEP[1] = true else Core.STEP[1] = false end
end
-----Режим АОсс
if Core.Calc_AI.Drum_AOss_Step >= 1  then
	for i= 0 , 23 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
	Core.STEP_STR[0] = "1.Останов зав. " 		--1 шаг
    Core.STEP_STR[1] = "2.Контур страв." 					--2 шаг
    Core.STEP_STR[2] = "3.Деблокировка" 					--3 шаг
if Core.Calc_AI.Drum_AOss_Step == 1 then Core.STEP[0] = true else Core.STEP[0] = false end
if Core.Calc_AI.Drum_AOss_Step == 2 then Core.STEP[1] = true else Core.STEP[1] = false end
if Core.Calc_AI.Drum_AOss_Step == 3 then Core.STEP[2] = true else Core.STEP[2] = false end
end

-----Режим АПК
if Core.Calc_AI.Drum_APK_Step >= 1  then
	for i= 0 , 23 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end	
	Core.STEP_STR[0] = "1.Звуковая сигнализация" 			--1 шаг
    Core.STEP_STR[1] = "2.Пуск МД, Пуск МН" 				--2 шаг
	Core.STEP_STR[2] = "3.Заполнение нагнетателя" 			--3 шаг
    Core.STEP_STR[3] = "4.Пуск ТГ" 							--4 шаг
    Core.STEP_STR[4] = "5.Запуск двигателя" 				--5 шаг
    Core.STEP_STR[5] = "6.Прогрев двигателя" 				--6 шаг
if Core.Calc_AI.Drum_APK_Step == 1 then Core.STEP[0] = true else Core.STEP[0] = false end
if Core.Calc_AI.Drum_APK_Step == 2 then Core.STEP[1] = true else Core.STEP[1] = false end
if Core.Calc_AI.Drum_APK_Step == 3 then Core.STEP[2] = true else Core.STEP[2] = false end
if Core.Calc_AI.Drum_APK_Step == 4 then Core.STEP[3] = true else Core.STEP[3] = false end
if Core.Calc_AI.Drum_APK_Step == 5 then Core.STEP[4] = true else Core.STEP[4] = false end
if Core.Calc_AI.Drum_APK_Step == 6 then Core.STEP[5] = true else Core.STEP[5] = false end
end

-----Режим АПМ
if Core.Calc_AI.Drum_APM_Step >= 1  then
	for i= 0 , 23 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end	
	Core.STEP_STR[0] = "1.Звуковая сигнализация" 			--1 шаг
    Core.STEP_STR[1] = "2.Пуск МД, Пуск МН" 				--2 шаг
	Core.STEP_STR[2] = "3.Заполнение нагнетателя" 			--3 шаг
    Core.STEP_STR[3] = "4.Пуск ТГ" 							--4 шаг
    Core.STEP_STR[4] = "5.Запуск двигателя" 				--5 шаг
    Core.STEP_STR[5] = "6.Прогрев двигателя" 				--6 шаг
    Core.STEP_STR[6] = "7.Переход К-М" 						--7 шаг
if Core.Calc_AI.Drum_APM_Step == 1 then Core.STEP[0] = true else Core.STEP[0] = false end
if Core.Calc_AI.Drum_APM_Step == 2 then Core.STEP[1] = true else Core.STEP[1] = false end
if Core.Calc_AI.Drum_APM_Step == 3 then Core.STEP[2] = true else Core.STEP[2] = false end
if Core.Calc_AI.Drum_APM_Step == 4 then Core.STEP[3] = true else Core.STEP[3] = false end
if Core.Calc_AI.Drum_APM_Step == 5 then Core.STEP[4] = true else Core.STEP[4] = false end
if Core.Calc_AI.Drum_APM_Step == 6 then Core.STEP[5] = true else Core.STEP[5] = false end
if Core.Calc_AI.Drum_APM_Step == 7 then Core.STEP[6] = true else Core.STEP[6] = false end
end
-----Режим ПБЗ
if Core.Calc_AI.Drum_PBZ_Step >= 1  then
	for i= 0 , 23 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end	
	Core.STEP_STR[0] = "1.Звуковая сигнализация" 			--1 шаг
    Core.STEP_STR[1] = "2.Пуск МД, Пуск МН" 				--2 шаг
    Core.STEP_STR[2] = "3.Пуск ТГ" 							--3 шаг
    Core.STEP_STR[3] = "4.Запуск двигателя" 				--4 шаг
if Core.Calc_AI.Drum_PBZ_Step == 1 then Core.STEP[0] = true else Core.STEP[0] = false end
if Core.Calc_AI.Drum_PBZ_Step == 2 then Core.STEP[1] = true else Core.STEP[1] = false end
if Core.Calc_AI.Drum_PBZ_Step == 3 then Core.STEP[2] = true else Core.STEP[2] = false end
if Core.Calc_AI.Drum_PBZ_Step == 4 then Core.STEP[3] = true else Core.STEP[3] = false end
end

-----Режим ХП
if Core.Calc_AI.Drum_HP_Step >= 1  then
	for i= 0 , 23 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
	Core.STEP_STR[0] = "1.Звуковая сигнализация" 			--1 шаг
    Core.STEP_STR[1] = "2.Пуск МД, Пуск МН" 				--2 шаг
	Core.STEP_STR[2] = "3.1СК КУ1,КУ2,вкл." 				--3 шаг
    Core.STEP_STR[3] = "4.Стартер работает" 				--4 шаг
    Core.STEP_STR[4] = "5.Выбег роторов" 					--5 шаг
if Core.Calc_AI.Drum_HP_Step == 1 then Core.STEP[0] = true else Core.STEP[0] = false end
if Core.Calc_AI.Drum_HP_Step == 2 then Core.STEP[1] = true else Core.STEP[1] = false end
if Core.Calc_AI.Drum_HP_Step == 3 then Core.STEP[2] = true else Core.STEP[2] = false end
if Core.Calc_AI.Drum_HP_Step == 4 then Core.STEP[3] = true else Core.STEP[3] = false end
if Core.Calc_AI.Drum_HP_Step == 5 then Core.STEP[4] = true else Core.STEP[4] = false end
end
-----Режим ТП
if Core.Calc_AI.Drum_TP_Step >= 1  then
	for i= 0 , 23 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
	Core.STEP_STR[0] = "1.Звуковая сигнализация" 			--1 шаг
    Core.STEP_STR[1] = "2.Пуск МД, Пуск МН" 				--2 шаг
	Core.STEP_STR[2] = "3.1СК КУ1,КУ2,вкл." 				--3 шаг
    Core.STEP_STR[3] = "4.Стартер работает" 				--4 шаг
    Core.STEP_STR[4] = "5.Выбег роторов" 					--5 шаг
if Core.Calc_AI.Drum_TP_Step == 1 then Core.STEP[0] = true else Core.STEP[0] = false end
if Core.Calc_AI.Drum_TP_Step == 2 then Core.STEP[1] = true else Core.STEP[1] = false end
if Core.Calc_AI.Drum_TP_Step == 3 then Core.STEP[2] = true else Core.STEP[2] = false end
if Core.Calc_AI.Drum_TP_Step == 4 then Core.STEP[3] = true else Core.STEP[3] = false end
if Core.Calc_AI.Drum_TP_Step == 5 then Core.STEP[4] = true else Core.STEP[4] = false end
end


-----Режим К-М
if Core.Calc_AI.Drum_K_M_Step >= 1  then
	for i= 0 , 23 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end	
	Core.STEP_STR[0] = "1.Переход К-М" 					--1 шаг
if Core.Calc_AI.Drum_K_M_Step == 1 then Core.STEP[0] = true else Core.STEP[0] = false end
end

-----Режим М-К
if Core.Calc_AI.Drum_M_K_Step >= 1  then
	for i= 0 , 23 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end	
	Core.STEP_STR[0] = "1.Переход М-К" 					--1 шаг
if Core.Calc_AI.Drum_M_K_Step == 1 then Core.STEP[0] = true else Core.STEP[0] = false end
end

-----Режим КПК
if Core.Calc_AI.Drum_KPK_Step >= 1  then
	for i= 0 , 23 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
	Core.STEP_STR[0] = "1.Звуковая сигнализация" 			--1 шаг
    Core.STEP_STR[1] = "2.Пуск МН" 							--2 шаг
	Core.STEP_STR[2] = "3.Заполнение нагнетателя"			--3 шаг
    Core.STEP_STR[3] = "4.Пуск ТГ" 							--4 шаг
	Core.STEP_STR[4] = "5.Кр.1,6 закр." 					--5 шаг
	Core.STEP_STR[5] = "6.Кр.12 закр. Кр.9 откр." 			--6 шаг
if Core.Calc_AI.Drum_KPK_Step == 1 then Core.STEP[0] = true else Core.STEP[0] = false end
if Core.Calc_AI.Drum_KPK_Step == 2 then Core.STEP[1] = true else Core.STEP[1] = false end
if Core.Calc_AI.Drum_KPK_Step == 3 then Core.STEP[2] = true else Core.STEP[2] = false end
if Core.Calc_AI.Drum_KPK_Step == 4 then Core.STEP[3] = true else Core.STEP[3] = false end
if Core.Calc_AI.Drum_KPK_Step == 5 then Core.STEP[4] = true else Core.STEP[4] = false end
if Core.Calc_AI.Drum_KPK_Step == 6 then Core.STEP[5] = true else Core.STEP[5] = false end
end

-----Режим НОбс
if Core.Calc_AI.Drum_NObs_Step >= 1  then
	for i= 0 , 23 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end	
	Core.STEP_STR[0] = "1.Переход М-К." 					--1 шаг
    Core.STEP_STR[1] = "2.Охлаждение Д" 					--2 шаг
    Core.STEP_STR[2] = "3.Останов Д" 						--3 шаг
if Core.Calc_AI.Drum_NObs_Step == 1 then Core.STEP[0] = true else Core.STEP[0] = false end
if Core.Calc_AI.Drum_NObs_Step == 2 then Core.STEP[1] = true else Core.STEP[1] = false end
if Core.Calc_AI.Drum_NObs_Step == 3 then Core.STEP[2] = true else Core.STEP[2] = false end
end

-----Режим НОcс
if Core.Calc_AI.Drum_NOss_Step >= 1  then
	for i= 0 , 23 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end	
	Core.STEP_STR[0] = "1.Переход М-К." 					--1 шаг
    Core.STEP_STR[1] = "2.Охлаждение Д" 					--2 шаг
    Core.STEP_STR[2] = "3.Останов Д" 		--3 шаг
    Core.STEP_STR[3] = "4.Контур страв." 		--3 шаг
if Core.Calc_AI.Drum_NOss_Step == 1 then Core.STEP[0] = true else Core.STEP[0] = false end
if Core.Calc_AI.Drum_NOss_Step == 2 then Core.STEP[1] = true else Core.STEP[1] = false end
if Core.Calc_AI.Drum_NOss_Step == 3 then Core.STEP[2] = true else Core.STEP[2] = false end
if Core.Calc_AI.Drum_NOss_Step == 4 then Core.STEP[3] = true else Core.STEP[3] = false end
end

-----Режим ограничение
if Core.Calc_AI.Drum_OGR_Step >= 1  then
	for i= 0 , 23 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
	Core.STEP_STR[0] = "1.Кр.2 закр.,Кр.6 откр. Дебл" 		--1 шаг
if Core.Calc_AI.Drum_OGR_Step == 1 then Core.STEP[0] = true else Core.STEP[0] = false end
end

-----Режим ПЗМ
if Core.Calc_AI.Drum_PZM_Step >= 1  then
	for i= 0 , 23 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end	
	Core.STEP_STR[0] = "1.Звуковая сигнализация" 			--1 шаг
 --   if TS.BTA_gidro == true then 
	Core.STEP_STR[1] = "2.Защиты PмН, PмД, dPм-г" 	
--	else 
--	Core.STEP_STR[1] = "2.Защиты PмН, PмД, dPм-г взвед." 
--	end	--2 шаг
	Core.STEP_STR[2] = "3.Аварии сработали"					--3 шаг
    Core.STEP_STR[3] = "4.Деблокировка" 					--4 шаг
if Core.Calc_AI.Drum_PZM_Step == 1 then Core.STEP[0] = true else Core.STEP[0] = false end
if Core.Calc_AI.Drum_PZM_Step == 2 then Core.STEP[1] = true else Core.STEP[1] = false end
if Core.Calc_AI.Drum_PZM_Step == 3 then Core.STEP[2] = true else Core.STEP[2] = false end
if Core.Calc_AI.Drum_PZM_Step == 4 then Core.STEP[3] = true else Core.STEP[3] = false end
end

else
for i= 0 , 23 do
Core.STEP_STR[i] = ""
Core.STEP[i] = false
end 
end


if
    Core.Calc_AI.sm_BleedingN_Step >= 1 or 
	Core.Calc_AI.sm_Check_Step >= 1 or
	Core.Calc_AI.sm_CoolingD_Step >= 1 or 
	Core.Calc_AI.sm_fillingN_Step >= 1 or
	Core.Calc_AI.sm_K_M_Step >= 1 or 
	Core.Calc_AI.sm_M_K_Step >= 1 or
	Core.Calc_AI.sm_Sound_Step >= 1 or 
	Core.Calc_AI.sm_startD_Step >= 1 or
	Core.Calc_AI.sm_startMD_Step >= 1 or 
	Core.Calc_AI.sm_startMN_Step >= 1 or
	Core.Calc_AI.sm_Stop_Step >= 1 or
    Core.Calc_AI.sm_startTG_Step >=1 or
	Core.Calc_AI.sm_warmD_Step >=1
then

-- Подрежим Звуковая сигнализация
if Core.Calc_AI.sm_Sound_Step >= 1 then
	Core.STEP_STR[9] = "1.Звуковая сигнализация" 							--1 шаг
if Core.Calc_AI.sm_Sound_Step == 1 then Core.STEP[9] = true else Core.STEP[9] = false end  
if Core.Calc_AI.sm_Sound_Step == 2 then 
for i= 9 , 9 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
end
end

-- Подрежим Пуск ТГ
if Core.Calc_AI.sm_startTG_Step >= 1 then
	Core.STEP_STR[9] = "1.Кр.12 откр.Продувка" 							--1 шаг
	Core.STEP_STR[10] = "2.Кр.9 закр.Пуск ТГ" 							--2 шаг
if Core.Calc_AI.sm_startTG_Step == 1 then Core.STEP[9] = true else Core.STEP[9] = false end
if Core.Calc_AI.sm_startTG_Step == 2 then Core.STEP[10] = true else Core.STEP[10] = false end  
if Core.Calc_AI.sm_startTG_Step == 3 then 
for i= 9 , 10 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
end
end

-- Подрежим Прогрев
if Core.Calc_AI.sm_warmD_Step >= 1 then
	Core.STEP_STR[9] = "1.Прогрев на ХХ" 								--1 шаг
	Core.STEP_STR[10] = "2.Выход на НАРР" 								--2 шаг
	Core.STEP_STR[11] = "3.Прогрев на НАРР" 							--3 шаг
if Core.Calc_AI.sm_warmD_Step == 1 then Core.STEP[9] = true else Core.STEP[9] = false end
if Core.Calc_AI.sm_warmD_Step == 2 then Core.STEP[10] = true else Core.STEP[10] = false end  
if Core.Calc_AI.sm_warmD_Step == 3 then Core.STEP[11] = true else Core.STEP[11] = false end  
if Core.Calc_AI.sm_warmD_Step == 4 then 
for i= 9 , 11 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
end
end
-- Подрежим Пуск МН
if Core.Calc_AI.sm_startMN_Step >=1 then
	Core.STEP_STR[9] = "1.ПНСН  вкл. Pм Н > Уст."					--1 шаг
  	Core.STEP_STR[10] = "2.Стаб. Защиты Pмсм Н взв."					--2 шаг
  	Core.STEP_STR[11] = "3.dРм-г>Уст"								--3 шаг
  	Core.STEP_STR[12] = "4.Защиты Pм-г взв."					--4 шаг
if Core.Calc_AI.sm_startMN_Step ==1 then Core.STEP[9] = true  else Core.STEP[9] = false end 
if Core.Calc_AI.sm_startMN_Step ==2 then Core.STEP[10] = true  else Core.STEP[10] = false end 
if Core.Calc_AI.sm_startMN_Step ==3 then Core.STEP[11] = true  else Core.STEP[11] = false end 
if Core.Calc_AI.sm_startMN_Step ==4 then Core.STEP[12] = true  else Core.STEP[12] = false end 
if Core.Calc_AI.sm_startMN_Step ==5 then 
for i= 9 , 12 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
end
end

-- Подрежим Пуск МД
if Core.Calc_AI.sm_startMD_Step >=1 then
--if TS.BTA_gidro == true then 
	Core.STEP_STR[14] = "1.МНОД, МННД вкл. "								--1 шаг
  	Core.STEP_STR[15] = "2.PмД>Уст"							--2 шаг
 	Core.STEP_STR[16] = "3.Защ.PмД до ХХ вз."				--3 шаг
--else  
--	Core.STEP_STR[12] = "1.ПНО  вкл. "								--1 шаг
--  	Core.STEP_STR[13] = "2.PмД>Уст "								--2 шаг
--  	Core.STEP_STR[14] = "3.Защита PмсмД вз. ПНСД вкл."				--3 шаг
--end	
if Core.Calc_AI.sm_startMD_Step ==1 then Core.STEP[14] = true  else Core.STEP[14] = false end 
if Core.Calc_AI.sm_startMD_Step ==2 then Core.STEP[15] = true  else Core.STEP[15] = false end 
if Core.Calc_AI.sm_startMD_Step ==3 then Core.STEP[16] = true  else Core.STEP[16] = false end
if Core.Calc_AI.sm_startMD_Step ==4 then   
for i= 14 , 16 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
end
end


-- Подрежим Заполнение Н
if Core.Calc_AI.sm_fillingN_Step >=1 then
	Core.STEP_STR[9] = "1.Кр.4 откр." 					--1 шаг
  	Core.STEP_STR[10] = "2.Кр.5 закр. Продувка "			--2 шаг
  	Core.STEP_STR[11] = "3.Стабилизация"		--3 шаг
  	Core.STEP_STR[12] = "4.Кр.1 откр."				--4 шаг
  	Core.STEP_STR[13] = "5.Кр.4 закр."		--5 шаг
  	Core.STEP_STR[14] = "6.Кр.6 откр."		--6 шаг
if Core.Calc_AI.sm_fillingN_Step ==1 then Core.STEP[9] = true  else Core.STEP[9] = false end
if Core.Calc_AI.sm_fillingN_Step ==2 then Core.STEP[10] = true  else Core.STEP[10] = false end
if Core.Calc_AI.sm_fillingN_Step ==3 then Core.STEP[11] = true  else Core.STEP[11] = false end
if Core.Calc_AI.sm_fillingN_Step ==4 then Core.STEP[12] = true  else Core.STEP[12] = false end
if Core.Calc_AI.sm_fillingN_Step ==5 then Core.STEP[13] = true  else Core.STEP[13] = false end
if Core.Calc_AI.sm_fillingN_Step ==6 then Core.STEP[14] = true  else Core.STEP[14] = false end
if Core.Calc_AI.sm_fillingN_Step ==7 then 
for i= 9 , 14 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
end
end

-- Подрежим Стравливание К
if Core.Calc_AI.sm_BleedingN_Step >=1 then
	Core.STEP_STR[9] = "1.Кр.1, 2, 6, 4 закр."			--1 шаг
  	Core.STEP_STR[10] = "2.Кр.5 откр., Нет газа в К"		--2 шаг
  	Core.STEP_STR[11] = "3.Защита PмсмН,dРм-г снять"		--3 шаг
if Core.Calc_AI.sm_BleedingN_Step ==1 then Core.STEP[9] = true  else Core.STEP[9] = false end 
if Core.Calc_AI.sm_BleedingN_Step ==2 then Core.STEP[10] = true  else Core.STEP[10] = false end 
if Core.Calc_AI.sm_BleedingN_Step ==3 then Core.STEP[11] = true  else Core.STEP[11] = false end
if Core.Calc_AI.sm_BleedingN_Step ==4 then   
for i= 9 , 11 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
end
end


-- Подрежим Останов ГПУ
if Core.Calc_AI.sm_Stop_Step >=1 then
	Core.STEP_STR[13] = "1.Кр.12 закр., СК закр."			--1 шаг
  	Core.STEP_STR[14] = "2.Кр.1,2,6,4 закр,Nкнд,Nквд<300"		--2 шаг
if Core.Calc_AI.sm_Stop_Step ==1 then Core.STEP[13] = true  else Core.STEP[13] = false end 
if Core.Calc_AI.sm_Stop_Step ==2 then Core.STEP[14] = true  else Core.STEP[14] = false end 
if Core.Calc_AI.sm_Stop_Step ==3 then   
for i= 13 , 14 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
end
end

-- Подрежим Контроль САУ
if Core.Calc_AI.sm_Check_Step >=1 then
	Core.STEP_STR[9] = "1.Контроль САУ"			--1 шаг
if Core.Calc_AI.sm_Check_Step ==1 then Core.STEP[9] = true  else Core.STEP[9] = false end 
if Core.Calc_AI.sm_Check_Step ==2 then   
for i= 9 , 9 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
end
end

-- Подрежим Охлаждение Д
if Core.Calc_AI.sm_CoolingD_Step >=1 then
	Core.STEP_STR[9] = "1.Выход на МГ"			--1 шаг
  	Core.STEP_STR[10] = "2.Охлаждение на МГ"	--2 шаг
	Core.STEP_STR[11] = "3.Выход на ХХ"			--3 шаг
  	Core.STEP_STR[12] = "4.Охлаждение на ХХ"	--4 шаг
if Core.Calc_AI.sm_CoolingD_Step ==1 then Core.STEP[9] = true  else Core.STEP[9] = false end 
if Core.Calc_AI.sm_CoolingD_Step ==2 then Core.STEP[10] = true  else Core.STEP[10] = false end 
if Core.Calc_AI.sm_CoolingD_Step ==3 then Core.STEP[11] = true  else Core.STEP[11] = false end 
if Core.Calc_AI.sm_CoolingD_Step ==4 then Core.STEP[12] = true  else Core.STEP[12] = false end
if Core.Calc_AI.sm_CoolingD_Step ==5 then   
for i= 9 , 12 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
end
end

-- Подрежим К-М
if Core.Calc_AI.sm_K_M_Step >=1 then
	Core.STEP_STR[9] = "1.Кр.2 откр."			--1 шаг
  	Core.STEP_STR[10] = "2.Кр.6 закр."			--2 шаг
if Core.Calc_AI.sm_K_M_Step ==1 then Core.STEP[9] = true  else Core.STEP[9] = false end 
if Core.Calc_AI.sm_K_M_Step ==2 then Core.STEP[10] = true  else Core.STEP[10] = false end 
if Core.Calc_AI.sm_K_M_Step ==3 then   
for i= 9 , 10 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
end
end


-- Подрежим М-К
if Core.Calc_AI.sm_M_K_Step >=1 then
	Core.STEP_STR[9] = "1.Кр.6 откр."			--1 шаг
  	Core.STEP_STR[10] = "2.Кр.2 закр."			--2 шаг
if Core.Calc_AI.sm_M_K_Step ==1 then Core.STEP[9] = true  else Core.STEP[9] = false end 
if Core.Calc_AI.sm_M_K_Step ==2 then Core.STEP[10] = true  else Core.STEP[10] = false end 
if Core.Calc_AI.sm_M_K_Step ==3 then   
for i= 9 , 10 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
end
end

-- Подрежим Запуск Д
if Core.Calc_AI.sm_startD_Step >=1 then
	Core.STEP_STR[9] = "1. 170c.РУС вкл." 						--1 шаг
  	Core.STEP_STR[10] = "2. 185с.РПС вкл."						--2 шаг
  	Core.STEP_STR[11] = "3. 193с.СПВ,КПГ вкл."					--3 шаг
  	Core.STEP_STR[12] = "4. 196с.СК откр."						--4 шаг
  	Core.STEP_STR[13] = "5. 204с.ТРК откр."						--5 шаг
  	Core.STEP_STR[14] = "6. 285с.Nкнд >2150"					--6 шаг
	Core.STEP_STR[15] = "7. Nквд > 6200,ХХ" 					--7 шаг

if Core.Calc_AI.sm_startD_Step ==1 then Core.STEP[9] = true  else Core.STEP[9] = false end
if Core.Calc_AI.sm_startD_Step ==2 then Core.STEP[10] = true  else Core.STEP[10] = false end
if Core.Calc_AI.sm_startD_Step ==3 then Core.STEP[11] = true  else Core.STEP[11] = false end
if Core.Calc_AI.sm_startD_Step ==4 then Core.STEP[12] = true  else Core.STEP[12] = false end
if Core.Calc_AI.sm_startD_Step ==5 then Core.STEP[13] = true  else Core.STEP[13] = false end
if Core.Calc_AI.sm_startD_Step ==6 then Core.STEP[14] = true  else Core.STEP[14] = false end
if Core.Calc_AI.sm_startD_Step ==7 then Core.STEP[15] = true  else Core.STEP[15] = false end
if Core.Calc_AI.sm_startD_Step ==9 then

for i= 9 , 15 do
	Core.STEP_STR[i] = ""
	Core.STEP[i] = false
	end
end
end

else
for i= 9 , 23 do
Core.STEP_STR[i] = ""
Core.STEP[i] = false
end 

end
end


-------------------------------------------------------------------------------------------
function GetStatusTMR(DBname_t,DBname_t1, firstID_t,appName_t)

local TagName_t = {
'TMR.Check_SAU_12h',
'TMR.Check_SAU_12s',
'TMR.Check_SAU_6s',
'TMR.Check_SAU_4s',
'TMR.CoolingGTD_NARR',
'TMR.CoolingGTD_HH',
'TMR.OpenKPG',
'TMR.OpenSK',
'TMR.RazgonGT',
'TMR.Sound_ON',
'TMR.StartGTD_1',
'TMR.is_flame',
'TMR.prodN',
'TMR.stabMN',
'TMR.stabMUN',
'TMR.stabN',
'TMR.startMD',
'TMR.warm_HH',
'TMR.warm_NARR',
'TMR.ES_Work_HP',
'TMR.ES_Work_TP',
'TMR.KAO_ON',
'TMR.KPG_1s',
'TMR.KPG_1_1s',
'TMR.PRU_NObs',
'TMR.PRU_NOss',
'TMR.PRU_AObs',
'TMR.PRU_AOss',
'TMR.KPK',
'TMR.CoolingStab',
'TMR.Check_Chips',
'TMR.StabMD',
'TMR.startTG',
'TMR.SK_ON',
'TMR.RUS_ON',
'TMR.RPS_ON',
'TMR.BP_ON',
'TMR.BP_OF',
'TMR.SK_Control',
'TMR.Filling_oil_Kr1',
'TMR.Filling_oil_Kr2',
'TMR.KPR1_ON',
'TMR.KPR2_ON',
'TMR.BP_SPV_OF',
'TMR.ORT',
'TMR.MNU_OF',
'TMR.Modes',
'TMR.Pm_D',
'TMR.Pm_MNOD_004',
'TMR.Pm_MNOD',
'TMR.MNOD_PZM_OF',
'TMR.MNOD_OF',
'TMR.Pm_in_D_002',
'TMR.Gas_N1',
'TMR.Gas_N2'


	}

local TagComment_t = {}
local ConString_t ={}

local TagName_chang_t ={
'ustT.Check_SAU_12h',
'ustT.Check_SAU_12s',
'ustT.Check_SAU_6s',
'ustT.Check_SAU_4s',
'ustT.CoolingGTD_NARR',
'ustT.CoolingGTD_HH',
'ustT.OpenKPG',
'ustT.OpenSK',
'ustT.RazgonGT',
'ustT.Sound_ON',
'ustT.StartGTD_1',
'ustT.is_flame',
'ustT.prodN',
'ustT.stabMN',
'ustT.stabMUN',
'ustT.stabN',
'ustT.startMD',
'ustT.warm_HH',
'ustT.warm_NARR',
'ustT.ES_Work_HP',
'ustT.ES_Work_TP',
'ustT.KAO_ON',
'ustT.KPG_1s',
'ustT.KPG_1_1s',
'ustT.PRU_NObs',
'ustT.PRU_NOss',
'ustT.PRU_AObs',
'ustT.PRU_AOss',
'ustT.KPK',
'ustT.CoolingStab',
'ustT.Check_Chips',
'ustT.StabMD',
'ustT.startTG',
'ustT.SK_ON',
'ustT.RUS_ON',
'ustT.RPS_ON',
'ustT.BP_ON',
'ustT.BP_OF',
'ustT.SK_Control',
'ustT.Filling_oil_Kr1',
'ustT.Filling_oil_Kr2',
'ustT.KPR1_ON',
'ustT.KPR2_ON',
'ustT.BP_SPV_OF',
'ustT.ORT',
'ustT.MNU_OF',
'ustT.Modes',
'ustT.Pm_D',
'ustT.Pm_MNOD_004',
'ustT.Pm_MNOD',
'ustT.MNOD_PZM_OF',
'ustT.MNOD_OF',
'ustT.Pm_in_D_002',
'ustT.Gas_N1',
'ustT.Gas_N2'



					
					}
local Status_t

for i=1, #TagName_t do
table.insert (TagComment_t, Core.getSignalComment(TagName_t[i]))
end


for j = 1 , #TagName_t  do
  
  if Core[""..TagName_t[j]..""] ~= Core[""..TagName_chang_t[j]..""]  and  Core[""..TagName_t[j]..""] ~= 0
     then  Status_t = string.gsub(tostring(Core[""..TagName_chang_t[j]..""]  - Core[""..TagName_t[j]..""]),"(%p)(.+)","" )                                  
           table.insert( ConString_t, TagComment_t[j].." - "..Status_t) end;
end;
Core["Status_Name_TMR"] = table.concat(ConString_t, "\n") 


end









local function get_ust(par)
 --if Core['Q_ON.TG'] == true then Core.directSet("10.50.50.234:10004", 1, 4294967289, 0, false,"BOOL" ) else Core.directSet("10.50.50.234:10004", 1, 4294967289, 0, true,"BOOL" )  end
 UnitState    =  GetMode() 
 TS   =  GetStatus() 
 Drum = drum()
 TMR = GetStatusTMR()
  local StatePLC =''
  local _,StPLC,_,_,_ = Core.directGet('10.50.50.234:10000', 1, '@STATE', 0)
  if StPLC ~= nil then StatePLC = StPLC end
  if StatePLC == '' then Core['State_PLC']= true else Core['State_PLC']=false end

  local StateSiemens =''
  local _,StSiemens,_,_,_ = Core.directGet('10.50.50.101:10008 ', 1, '@MESSAGE', 0)
  if StSiemens ~= nil then StateSiemens = StSiemens end
  if StateSiemens ~= '' then Core['State_Siemens']= true else Core['State_Siemens']=false end
end



Core.onTimer(3, 1,get_ust,true, true )
Core.waitEvents();
