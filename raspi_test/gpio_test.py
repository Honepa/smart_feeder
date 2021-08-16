from flask import Flask, render_template, redirect
import gpiozero
#import vlc
from time import sleep
import os
import pyaudio
import wave

app = Flask(__name__)

FORMAT = pyaudio.paInt16
CHANNELS = 1
RATE = 44100
CHUNK = 512
RECORD_SECONDS = 10
WAVE_OUTPUT_FILENAME = "/home/pi/smart_feeder/raspi_test/static/record.wav"
device_index = 1
audio = pyaudio.PyAudio()

b1 = gpiozero.LED(27)
b2 = gpiozero.LED(22)

b2.off()
b1.off()

knok = gpiozero.Button("GPIO17")

playing = set([1, 2, 3, 4])

@app.route('/')
def hello_world():
    return render_template('index.html')

@app.route('/run/')
def run():
    b1.on()
    k = 0
    while k < 2:
        b1.on()
        print(k)
        if not knok.value:
            k += 1
            while not knok.value:
                print(k)
    b1.off()
    return redirect('/', code=302)

@app.route('/play/')
def play():
    #print(1)
    #player = vlc.MediaPlayer("/home/pi/music/cat.mp3")
    #print(1)
    #player.play()
    #sleep(2)
    #while 1:
    #    x = player.get_state() 
    #    if x not in {1, 2, 3, 4}:
    #        break
    #print(1, x)
    #player.stop()
    #del player
    os.system("su pi -c 'mpg123 /home/pi/music/cat.mp3'")
    #sleep(2)
    return redirect('/', code=302)

@app.route('/record/')
def record():
    stream = audio.open(format=FORMAT, channels=CHANNELS,
                rate=RATE, input=True,input_device_index = device_index,
                frames_per_buffer=CHUNK)
    print ("recording started")
    Recordframes = []
 
    for i in range(0, int(RATE / CHUNK * RECORD_SECONDS)):
        data = stream.read(CHUNK)
        Recordframes.append(data)
    print ("recording stopped")
 
    stream.stop_stream()
    stream.close()
    audio.terminate()
 
    waveFile = wave.open(WAVE_OUTPUT_FILENAME, 'wb')
    waveFile.setnchannels(CHANNELS)
    waveFile.setsampwidth(audio.get_sample_size(FORMAT))
    waveFile.setframerate(RATE)
    waveFile.writeframes(b''.join(Recordframes))
    waveFile.close()
    return redirect('/', code=302)
    
if __name__ == '__main__':
    app.run()
