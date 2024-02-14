from flask import Flask, request, make_response

app = Flask(__name__)

@app.route('/')
def hello_world():
  return 'Hello, World!'

@app.route('/api/v1/bright', methods=['GET'])
def get_bright():
  resp = make_response({"bright": 3.4}, 200)
  resp.headers['Access-Control-Allow-Origin'] = '*'
  return resp

@app.route('/api/v1/datetime', methods=['GET', 'POST', 'OPTIONS'])
def post_datetime():
  if request.method == 'POST':
    app.logger.info('POST value = "%s"', request.get_data())    
    resp = make_response({
        "year": 2024, "month": 24, "day": 24,
        "hour": 24, "min": 24, "sec": 24
      }, 200)
    resp.headers['Access-Control-Allow-Origin'] = '*'
    return resp
  elif request.method == 'GET':
    app.logger.info('GET value = "%s"', request.get_data())    
    resp = make_response({
        "year": 2020, "month": 2, "day": 22,
        "hour": 22, "min": 22, "sec": 22
      }, 200)
    resp.headers['Access-Control-Allow-Origin'] = '*'
    return resp
  elif request.method == 'OPTIONS':
    app.logger.info('OPTIONS value = "%s"', request.get_data())    
    respo = make_response('OK', 204)
    respo.headers['Access-Control-Allow-Origin'] = '*'
    respo.headers['Access-Control-Request-Method'] = 'POST, GET, OPTIONS'
    respo.headers['Access-Control-Request-Headers'] = 'Content-Type, Authorization'
    return respo

if __name__ == '__main__':
    app.run(debug=True)
