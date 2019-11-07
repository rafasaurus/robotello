TODO:
*    full communication, with raspberry pi

Serial test:
*   minicom -b 9600 -o -D /dev/ttyAMA0
### Credits
@EinarArnason [ArduinoQueue](https://github.com/EinarArnason/ArduinoQueue)

### sound contrl via amixer
amixer scontrols
amixer sset 'Master' 50%

### pygame play the music

import pygame
pygame.mixer.init()
pygame.mixer.music.load("Music/myMp3file.mp3")
pygame.mixer.music.play()
while pygame.mixer.music.get_busy() == True:
continue
