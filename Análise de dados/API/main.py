from firebase_admin import initialize_app, db
from flask import Flask, send_file, request, Response
import plot

app = Flask(__name__)

#Função de retorno do gráfico de temperatura
@app.route('/plots/temp', methods=['GET'])
def graphs_temp():
    args = request.args
    datemin = args.get('datemin')
    datemax = args.get('datemax')
    medida = args.get('medida')
    resolucao = args.get('resolucao')
    bytes_obj = plot.do_plot_temp(datemin, datemax, medida, resolucao)
    
    return send_file(bytes_obj,
                    download_name='plot.png',
                    mimetype='image/png')

#função de retorno do gráfico de umidade
@app.route('/plots/umid', methods=['GET'])
def graphs_umid():
    args = request.args
    datemin = args.get('datemin')
    datemax = args.get('datemax')
    bytes_obj = plot.do_plot_umid(datemin, datemax) 
    
    return send_file(bytes_obj,
                    download_name='plot.png',
                    mimetype='image/png')
    
#função de retorno do arquivo de download
@app.route('/plots/download', methods=['GET'])
def download_file():
    args = request.args
    tipo = args.get('formato')
    tempo = args.get('periodo')

    arquivo, formato = plot.download(tipo, tempo)

    return Response(arquivo,
                    mimetype=formato,
                    headers={
                    'Content-Disposition': f'attachment; filename="dados.{tipo}"'
                    })


if __name__ == '__main__':
    app.run(debug=True)