pipeline {
  agent {
    docker {
      image 'jherr029/cpp-app:latest'
      args '-it -v $PWD:/app'
    }

  }
  stages {
    stage('Build') {
      steps {
        sh '''mkdir build
        cd build && cmake .. && make && ../bin/lytics'''
      }
    }
  }
  // environment {
  //   REDDIT_LOGIN = '${env.REDDIT_LOGIN}'
  //   REDDIT_PASS = '${env.REDDIT_PASS}'
  //   REDDIT_SECRET = '${env.REDDIT_SECRET}'
  //   REDDIT_KEY = '${env.REDDIT_KEY}'
  //   TWITCH_KEY = '${env.TWITCH_KEY}'
  //   TWITCH_SECRET = '${env.TWITCH_SECRET}'
  //   MYSQL_SERVER_REMOTE = '${env.MYSQL_SERVER_REMOTE}'
  //   MYSQL_LOGIN = '${env.MYSQL_LOGIN}'
  //   MYSQL_PASS = '${env.MYSQL_PASS}'
  // }
}