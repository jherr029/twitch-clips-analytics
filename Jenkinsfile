pipeline {
  agent {
    docker {
      image 'cpp-app'
      args '-it --rm -v $(pwd):/app --env-file app-env.list'
    }

  }
  stages {
    stage('Build') {
      steps {
        sh '''mkdir build
sudo rm -r build/*
cd build
cmake .. && make
../bin/lytics'''
      }
    }
  }
}