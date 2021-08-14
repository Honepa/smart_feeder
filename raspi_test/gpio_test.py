from flask import Flask, render_template, redirect
import gpiozero
import vlc
from time import sleep

app = Flask(__name__)


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
    player = vlc.MediaPlayer("/home/pi/music/cat.mp3")
    player.play()
    sleep(2)
    while 1:
        x = player.get_state() 
        if x not in {1, 2, 3, 4}:
            break
    print(1, x)
    player.stop()
    del player
    return redirect('/', code=302)

if __name__ == '__main__':
    app.run()