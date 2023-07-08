# MQTT Acquisition System
Este projeto Django realiza a comunicação com dispositivos via MQTT, possibilitando o cadastro do dispositivo e já tendo configurado o modelo de aquisição dinâmico.

## Instalação
Certifique-se que possui Python3 e PIP 3 instalados.

### 1. Criação do ambiente virtual
    
É fortemente recomendado que seja utilizado um ambiente virtual para a utilização neste projeto. Principalmente quando se têm outros projetos em Python 3 na mesma máquina. Para tanto, se já possuir o módulo do python "venv" instalado, basta rodar o comando:

Linux:

    python3 -m venv [nome do ambiente virtual]


Windows:

    python -m venv [nome do ambiente virtual]


### 2. Instalando os pacotes necessários
Para instalar os pacotes necessários vamos primeiro ir pelo terminal até a raíz do projeto. Então basta rodar o seguinte comando:

Linux:
    
    pip3 install -r requirements.txt


Windows:

    pip install -r requirements.txt

#### Opcional

Para realizar testes/desenvolvimento em máquina local, você pode instalar o Mosquitto Broker (Caso não tenha um Broker externo), e o MQTT Explorer para testes e visualização de fluxo.

### 3. Configurando o projeto
Iremos utilizar dois principais arquivos de configuração, a dizer *conf/settings.py* e *conf/local_settings.py*. O primeiro contém todas as configurações que serão utilizadas no projeto em qualquer máquina que este rode, já o outro contém as configurações específicas para cada máquina.

Portando, na instalação devemos apenas criar um arquivo chamado *local_settings.py* na pasta *conf*, para configurarmos o projeto. Este arquivo já é esperado para ser importado no settings.py e irá sobrescrever qualquer configuração existente.

Segue um exemplo de *local_settings.py*:

    DEBUG = True

    BASE_URL = "http://teste123.com.br/"
    BROKER_ADDRESS = "127.0.0.1"
    
    ALLOWED_HOSTS = ["subdominio.dominio.com.br", "123.45.6.789" ]
    
    DATABASES = {
        'default': {
            'ENGINE': 'django.db.backends.postgresql_psycopg2',
            'NAME': 'nome_do_banco_de_dados',
            'USER': 'postgres',
            'PASSWORD': 'senha',
            'HOST': 'localhost',
            'PORT': 5432
        }
    }
    
Abaixo, uma definição rápida de cada elemnto:
#### DEBUG
Define o modo de operação do Django. Se definido como True, o djando irá operar em modo DEBUG e exibirá informação mais detalahdas sobre requisições quando houver algum erro. 

###### ATENÇÃO: em modo de produção manter DEBUG = False

#### ALLOWED_HOSTS
Uma lista de todos os domínios ou IPs que o servidor deve poder atuar. Ao rodar o servidor pelo Django (* comandos do django) é possivel definir ["*", ] para aceitar qualquer host.

####  DATABASES
Um dicionário contendo as informações de cada base de dados que será utilizada. Por padrão o Django utiliza o database definido como *default*. A seguir a definição de cada valor:

    DATABASES = {
        'default': -> Identificação da base para o Django
            'ENGINE': 'django.db.backends.postgresql_psycopg2', -> módulo que realizara o gerenciamento da base
            'NAME': 'delta', ->  Nome da base
            'USER': 'postgres', -> Usuário
            'PASSWORD': '12345', -> Senha
            'HOST': 'localhost', -> Host
            'PORT': 5432, -> Porta
        }
    }

Além dessas podem ser definidas várias outras configurações de acordo com a documentação do Django.

### BASE_URL
Descreve o endereço que o servidor está hospedado. Isso, será utilizado para gerar os links de atualização de firmwares.

### BROKER_ADDRESS
Endereço do Broker MQTT que o servidor tentará conectar.

### Realizando as migrações

    ATENÇÂO: Neste projeto em específico, utilizamos funções assíncronas que
    são iniciadas juntamente ao django nno arquivo __init__.py dos apps accounts
    e mqtt_manager. Portanto é necessário comentar todo o código desses arquivos
    antes e rodar estes comandos a fim de evitar problemas.

    Para tanto, basta inserir um # no início de cada linha de código.

O Django possúi uma ótima ferramenta de gerenciamento do banco de dados. Básicamente executaremos dois comandos. O primeiro irá ler os arquivos *models* e criarão os arquivos de migração - que definem quais as alterações necessárias na base de dados. O segundo irá aplicar as alterações a partir dos arquivos criados com o primeiro comando. São os comandos:

Linux:

    python3 manage.py makemigrations
    python3 manage.py migrate

Windows:

    python manage.py makemigrations
    python manage.py migrate

### Executar
Perfeito, agora temos o servidor instalado e configurado. Para executá-lo podemos configurar o wsgi em um servidor (consultar documentação), ou executar o servidor próprio do Django, que serve muito bem para testes - esse servidor começa a ter problemas já com um número baixo de requisições simultâneas, por isso não é adequado para produção.

Para executar o servidor do Django, rodamos o seguinte comando:

Linux:

    python3 manage.py runserver 8000

Windows:

    python manage.py runserver 8000

Sendo 8000 a porta em que irá rodar.

---

## Instalação

Comandos importantes:

Sempre que alguma alteração for feita nos modelos de dados (models), é importante criar os arquivos de migrações e aplicálos na base de dados com os seguintes comandos.

Linux:

    python3 manage.py makemigrations
    python3 manage.py migrate

Windows:

    python manage.py makemigrations
    python manage.py migrate

Além disso, sempre que arquivos estáticos novos forem adicionados, quando se estiver em produção, é importando rodar o collectstatic que reúne os arquivos estáticos no diretório que o servidor (Ex: Apache) utilizará.

Linux:

    python3 manage.py collectstatic

Windows:

    python manage.py collectstatic


Por fim, vamos criar um superusuário, que terá todas as permissões dentro do sistema. Para tanto, executaremos os seguintes comandos:

Linux:

    python3 manage.py createsuperuser

Windows:

    python manage.py createsuperuser

## Fluxo de adição de um dispositivo
Dentro do sistema, um administrador pode cadastrar os dispositivos, indicando seu modelo, MAC, e localização, então poderá cadastrar os dados que serão enviados pelo dispositivo, indicando a tag que referenciará o dado. 

*OBS: Todos os dados são armazenados como pontos flutuantes de dupla precisão, através disso é possível abranger um número satisfatório de caso. Se não for suficiente para o seu caso, fique à vontade para editar este módulo.*

## FLuxo do sistema

Este projeto possui a arquitetura descrita à seguir:

    ---------------- <--- /in -----  ---------------- <--- /in ----  ----------------
    |  Dispositivo |                 |    Broker    |                |   Servidor   |
    ---------------- ---- /out --->  ---------------- ---- /out -->  ----------------

Para quem não está familiarizado com o MQTT, o Broker é um servidor que centraliza a comunicação, poupando sockets do nosso servidor principal. Dentro do Broker, temos os chamados "tópicos", que são canais de mensagem. Um cliente MQTT pode se inscrever em qualquer tópico "s", e então receberá as mensagens publicadas no topico "s". Além disso, um cliente pode publicar em um tópico "p" (independente de estar inscrito), e quando o fizer, todos os clientes inscritos no tópico "t" receberão a mensagem.

Com isso, percebemos que se publicarmos uma mensagem em um tópico em que estamos inscritos, receberemos a mensagem.

Assim sendo, temos a definimos a seguinte arquitetura:
    
- Cada dispositivo se inscreverá no tópico "sensor/\<mac>/in" (tópico de entrada do dispositivo), e publicará no tópico "sensor/\<mac>/out" (tópico de saída do dispositivo). Vale ressaltar que a notação \<mac> deverá ser substituida pelo MAC do dispositivo.
- Por sua vez, o servidor se inscreverá np tópico "sensor/\<mac>/out" de cada dispositivo cadastrado. E responderá mensagens, enviando a resposta no tópico "sensor/\<mac>/in".
- A fim de padronizar a comunicação, as mensagens trocadas deverão estar no formato JSON, e conter, pelo menos, a tag "version", indicando a versão do firmware.
- Sempre que, no servidor principal, houver um firmware cadastrado com uma versão superior à versão informada pelo firmware, o servidor responderá com a tag "update_url" que conterá um URL temporária para download do firmware.

---

## Documentação

Acesso à documentação do Django: https://docs.djangoproject.com/en/4.0/