from flask import Flask
from flask import request

app = Flask(__name__)

@app.route('/')
def hello_world():
  return 'Hello, World!'

@app.route('/api/v1/bright', methods=['GET'])
def get_bright():
    bright = {"bright": 3.4}
    return bright

@app.route('/api/v1/datetime', methods=['GET'])
def get_datetime():
    return {
        "year": 2020, "month": 2, "day": 22,
        "hour": 22, "min": 22, "sec": 22
    }

@app.route('/api/v1/datetime', methods=['POST'])
def post_datetime():
    bright = {"bright": 3.4}
    return {
        "year": 2020, "month": 2, "day": 22,
        "hour": 22, "min": 22, "sec": 22
    }

@app.route('/submit', methods=['POST'])
def submit():
    name = request.form['name']
    return f'Hello, {name}'

if __name__ == '__main__':
    app.run(debug=True)