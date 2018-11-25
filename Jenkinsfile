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
        dir(path: 'build') {
          sh 'cmake .. && make'
        }

      }
    }
    stage('Test') {
      steps {
        sh './bin/tester'
      }
    }
    stage('Run') {
      steps {
        sh './bin/lytics xa'
      }
    }
  }
}