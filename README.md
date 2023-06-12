# Projeto de Monitoramento Agrometeorológico usando IoT

Este repositório contém os códigos desenvolvidos durante o trabalho de conclusão de curso da graduação em Engenharia Elétrica pela Universidade Estadual de Londrina, em 2023.

## Autor

- Aluno: Mateus Perrut de Souza

## Orientador

- Prof. Maria Bernadete de Morais França

## Descrição do Projeto

O objetivo deste trabalho foi desenvolver um sistema IoT para o monitoramento de grandezas agrometeorológicas, levando em consideração o avanço no desenvolvimento de tecnologias específicas para IoT e a relevância da aplicação dessas tecnologias em sistemas de produção agrícola.

Foram implementados códigos para coleta e armazenamento em tempo real de dados agrometeorológicos, como temperatura, umidade do ar e intensidade luminosa, utilizando sensores especializados. Além disso, foi desenvolvida uma aplicação web que permite o acesso remoto e a visualização dos dados coletados.

A integração de API's possibilitou a combinação desses dados com outras fontes de informação, enriquecendo sua contextualização. Durante o estudo, enfrentamos desafios devido à complexidade das ferramentas integradas, porém, também identificamos os benefícios proporcionados pela implementação de novas tecnologias, abrindo perspectivas futuras de melhoria do sistema, como a adaptação a diferentes cultivos, inclusão de sensores adicionais, desenvolvimento de modelos preditivos e controle ambiental.

## Funcionalidades

- Coleta de dados agrometeorológicos em tempo real usando o microcontrolador ESP32 e os seguintes sensores:
  - SHT75: temperatura e umidade do ambiente
  - BH1750: luminosidade do ambiente
  - NEO-6M: módulo GPS operante com sinais de satélite

- Armazenamento dos dados coletados em um banco de dados em tempo real (Realtime Database Firebase).

- Aplicação web para acesso remoto e visualização dos dados coletados pelo sistema. A aplicação permite solicitar novas leituras em tempo real e plotar gráficos históricos de temperatura e umidade. Ela foi hospedada no servidor do Firebase.

- Análise dos dados coletados (API desenvolvida com Flask), incluindo a identificação de padrões sazonais por meio da análise temporal da temperatura e do histograma das faixas de umidade.

- Mecanismo de download dos dados coletados pelos sensores para análises mais específicas (API Flask).

Esse repositório contem os códigos desenvolvidos e a monográfia aprovada pela banca.
