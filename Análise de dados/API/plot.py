import firebase_admin
from firebase_admin import credentials
from firebase_admin import auth
from firebase_admin import db
import numpy as np
import pandas as pd
from datetime import datetime as dt
from datetime import timedelta
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import io
import PIL.Image as Image

#Objeto de credenciais do firebase
cred_obj = credentials.Certificate("C://arquivo com as credenciais.json")
firebase_admin.initialize_app(cred_obj, {
	'databaseURL': 'URL do banco de dados'
})

#Código para pegar o usuário do firebase
email = "email de usuário do firebase"
user = auth.get_user_by_email(email)
plt.style.use("bmh")

def do_plot_temp(datemin, datemax, medida, resolucao):
    #Conversão de data para timestamp
    datemin = dt.strptime(datemin, '%Y-%m-%dT%H:%M')
    datemax = dt.strptime(datemax, '%Y-%m-%dT%H:%M')

    #Conversão de data para timestamp
    datemin_timestamp = str(datemin.timestamp())
    datemax_timestamp = str(datemax.timestamp())

    #consulta dos dados do banco de dados
    ref = db.reference('/UsersData/'+ str(user.uid)+'/SHT/')
    query = ref.order_by_child('timestamp').start_at(datemin_timestamp).end_at(datemax_timestamp)
    data = query.get()

    #Criação do dataframe
    df = pd.DataFrame.from_dict(data, orient='index')
    df["data"] = pd.to_datetime(pd.to_numeric(df["timestamp"]), unit='s')
    df["TemperaturaC"] = df["TemperaturaC"].astype(float)
    df = df.drop(df[df['TemperaturaC'] < 0].index)

    #Criação do gráfico
    fig, ax = plt.subplots()
    fig.set_tight_layout(True)

    #Função de agregação dos dados de acordo com a frequência e a medida escolhidas
    if(medida == "all"):
        df_media = df.groupby(pd.Grouper(key='data', freq=resolucao)).agg({"TemperaturaC": "mean"}).reset_index()
        df_minimo = df.groupby(pd.Grouper(key='data', freq=resolucao)).agg({"TemperaturaC": "min"}).reset_index()
        df_maximo = df.groupby(pd.Grouper(key='data', freq=resolucao)).agg({"TemperaturaC": "max"}).reset_index()

        ax.plot(df_media["data"], df_media["TemperaturaC"], label="Média")
        ax.plot(df_minimo["data"], df_minimo["TemperaturaC"], label="Mínima")
        ax.plot(df_maximo["data"], df_maximo["TemperaturaC"], label="Média")
    else:
        df = df.groupby(pd.Grouper(key='data', freq=resolucao)).agg({"TemperaturaC": medida}).reset_index()
        ax.plot(df["data"], df["TemperaturaC"])
    
    data_final = dt.fromtimestamp(float(datemax_timestamp))
    
    #Configurações do gráfico
    ax.set_xlim([df['data'].iloc[0], data_final])
    ax.set_ylabel("Temperatura")
    plt.xticks(rotation=45)
    xtick_labels = ax.get_xticklabels()
    num_labels = len(xtick_labels)
    max_labels = 10 
    step = max(num_labels // max_labels, 1) 
    new_labels = [xtick_labels[i].get_text() for i in range(0, num_labels, step)]

    #Conversão do formato da data de acordo com a resolução escolhida
    if len(new_labels[0]) == 8 and new_labels[0][2] == "-" and new_labels[0][5] == " ":
        new_labels = [dt.strptime(date, '%m-%d %H').strftime("%d %H:%M") for date in new_labels]
    elif len(new_labels[0]) == 10 and new_labels[0][4] == "-" and new_labels[0][7] == "-":
        new_labels = [dt.strptime(date, '%Y-%m-%d').strftime("%d-%m") for date in new_labels]
        
    ax.set_xticklabels(new_labels)
    ax.set_xlabel("Tempo")

    #Salvando o gráfico em um buffer
    buf = io.BytesIO()
    fig.savefig(buf, format='png')
    buf.seek(0)
    return buf


def do_plot_umid(datemin, datemax):
    #Conversão de data para timestamp
    datemin = dt.strptime(datemin, '%Y-%m-%dT%H:%M')
    datemax = dt.strptime(datemax, '%Y-%m-%dT%H:%M')

    #Conversão de data para timestamp
    datemin_timestamp = str(datemin.timestamp())
    datemax_timestamp = str(datemax.timestamp())

    #consulta dos dados do banco de dados
    ref = db.reference('/UsersData/'+ str(user.uid)+'/SHT/')
    query = ref.order_by_child('timestamp').start_at(datemin_timestamp).end_at(datemax_timestamp)
    data = query.get()

    #Criação do dataframe
    df = pd.DataFrame.from_dict(data, orient='index')
    df["umidade"] = df["umidade"].astype(float)
    df = df.drop(df[df['umidade'] < 0].index)

    #Criação do gráfico
    fig, ax = plt.subplots()
    fig.set_tight_layout(True)

    #Configurações do gráfico
    bins_umid = list(range(0, 110, 10))
    ax.hist(df["umidade"], bins=bins_umid, rwidth=0.9, weights=np.zeros_like(df["umidade"]) + 1. / df["umidade"].size)
    ax.set_xlabel("Umidade")
    ax.set_ylabel("Frequência")

    #Salvando o gráfico em um buffer
    buf = io.BytesIO()
    fig.savefig(buf, format='png')
    buf.seek(0)
    return buf

def download(tipo, tempo):
    #Conversão de data para timestamp
    hoje = dt.now()
    hojeTimestamp = str(hoje.timestamp())

    #Determinando o período de consulta dos dados
    qnt = 1 if tempo == 'd' else 7 if tempo == 'w' else 30

    #Conversão de data para timestamp
    comeco = hoje - timedelta(days=qnt)
    comecoTimestamp = str(comeco.timestamp())

    #consulta dos dados do banco de dados
    ref = db.reference('/UsersData/'+ str(user.uid)+'/SHT/')
    query = ref.order_by_child('timestamp').start_at(comecoTimestamp).end_at(hojeTimestamp)
    data = query.get()

    #Criação do dataframe
    df = pd.DataFrame.from_dict(data, orient='index')
    df["TemperaturaC"] = df["TemperaturaC"].astype(float)
    df["data"] = pd.to_datetime(pd.to_numeric(df["timestamp"]), unit='s')
    df["umidade"] = df["umidade"].astype(float)
    df = df.drop(df[df['umidade'] < 0].index)
    df = df.drop(df[df['TemperaturaC'] < 0].index)  

    #Criação do arquivo de acordo com o formato escolhido
    if tipo == 'csv':
        arquivo = df.to_csv(index=False)
        mimetype = 'text/csv'
    elif tipo == 'json':
        arquivo = df.to_json(orient='records')
        mimetype = 'application/json'
    else:
        raise ValueError('Tipo de arquivo inválido')

    #Retorno do arquivo e do mimetype
    return arquivo, mimetype