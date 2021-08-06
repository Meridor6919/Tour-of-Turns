In this document you will find how to modify your some of your game files.

game_config.txt
Only first 3 lines of this file are relevent. 
By modifying the values here, you would affect some parameters of the application.
1. Default number of ais (from 0 to 7)
2. Your name (characters from 'a' to 'z' and from 'A' to 'Z' and '_' if you want a space. Max 20 characters)
3. Timer settings (from 0 to 60 Timer would be number you've written * 10 seconds. If 0 then timer is disabled)


window_config.txt
Only first 11 lines of this file are relevent. 
By modifying the values here, you would affect some parameters of the application.
1. Title (from 1 to 1024 characters)
2. Title theme name (form title_theme.txt)
3. Language (name of lang pack in Language directory)
4. AI module name (name of AI module in AI directory)
5. Windowed mode resolution X-axis(from 800 uppwards)
6. Window mode resolution Y-axis(from 600 uppwards)
7. Main color (from 3 to 15)
8. Secondary color (from 3 to 15, main color and secondary color cannot be the same)
9. Screen mode ( 0 = fullscreen, 1 = windowed)
10. Music volume (from 0.0 to 1.0)
11. Hamachi flag (0 = disabled, 1 = enabled)


title_themes.txt
Each line represents distinct title theme. Consequtive parameters are seperated by single tab.
Components with secondary color are displayed in place of first ' ' character in main color component.
If there are no spaces in main color component, then secondary color component is being draw on top of it starting from 1st character.
Don't leave empty lines at the end of the file!!!
1. Name (Max 20 characters)
2. Left component with main color (maximum size 20)
3. Right component with main color (maximum size 20)
4. Left component with secondary color (maximum size 20)
5. Right component with secondary color (maximum size 20)
6. Distance for title (between 3 and 10)
7. Wobble flag (wheater or not every second line should be shifted by one)


music.wav
You can change the music played in the game by substituting music.wav with your own musical file


Cars/
Each car is a text file saved with a '.car' extension. Consequtive parameters are saved in newlines.
All values are from 0 to nine digit numbers except visibility that is from 1 to 7
Don't add any new ones!!!
1. Maximum speed
2. Acceleration
3. Brake speed
4. Hand brake speed
5. Durability
6. Visibility
7. Turn modifier
8. Drift modifier


Tires/
Individual tires are saved in a text file with a '.tire' extension. Consequtive parameters are saved in newlines.
Each parameter is saved as (number_of_tests_to_pass, 'x' symbol, number_of_tests) numbers are from 1 to 20
1. Asphalt
2. Grass
3. Gravel
4. Sand
5. Mud
6. Ice
For example you can write '1x2' meaning that for terrain in the corresponding line player would need to pass 1 out of 2 tests


Tours/
Every map is saved with '.tour' extension. Consequtive parameters are saved in new line.
First parameters are cars that are available in the map. Simply put their names WITHOUT '.car' extension
After listing them all there should be an empty line and after that tour segments are saved
First character is terrain type and optionally you can add safe speed value (from 0 to 9 digit numbers) making it a turn.
Terrain types are identified as:
0 - Asphalt
1 - Grass
2 - Gravel
3 - Sand
4 - Mud
5 - Ice
For example straight part of the road on grass would be coded as '1' and turn on ice with the safe speed of 90 would be "590"


AI/
You can add new ai_modules into the AI directory, but be careful of not trusted sources.
Game doesn't sanitize .exe files so it can possibly run some malicious software.


LangPack/
LangPack contains as the name suggest language pack for the game. 
Default language is set in the window_config.txt file 