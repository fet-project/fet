#!/usr/bin/env groovy
def gitClone() {
  sh "rm -rf ci-build"
  checkout([$class: 'GitSCM',
            branches: [[name: '*/master']],
            doGenerateSubmoduleConfigurations: false,
            extensions: [[$class: 'RelativeTargetDirectory',
                          relativeTargetDir: 'ci-build']],
            submoduleCfg: [],
            userRemoteConfigs: [[url: 'https://github.com/fet-project/ci-build.git']]
  ])
}

def runContainer(String distro, String step) {
  echo "Running ${step} for ${distro}..."
  def imageName = "fetproject/${distro}:latest"
  def dockerWorkspace = '/tmp/workspace'
  def jobNameValid = env.JOB_NAME.replace('_', '')
  def containerName = "${jobNameValid}-${env.BUILD_ID}-${distro}"
  try {
    checkout scm
    gitClone()
    echo "Starting cointainer..."
    sh "/usr/bin/docker pull ${imageName}"
    sh "/usr/bin/docker run -d --volume=${env.WORKSPACE}:${dockerWorkspace}:rw --name ${containerName} ${imageName}"
    echo "Container started..."
    switch(step) {
      case 'codeAnalysis':
        sh "/usr/bin/docker exec ${containerName} ${dockerWorkspace}/ci-build/ci-build --fetdir ${dockerWorkspace} --fail-on-errors --code-analysis all"
        break
      case 'build':
        sh "/usr/bin/docker exec ${containerName} ${dockerWorkspace}/ci-build/ci-build --fetdir ${dockerWorkspace} --fail-on-errors --build"
        break
      default:
        assert false : "Unknown step!"
    }
  }
  finally {
    echo "Stopping container..."
    sh "/usr/bin/docker rm -f ${containerName}"
  }
}

def listBuilders(distros) {
  def builders = [:]
  // I tried distros.each() but does not work due to JENKINS-26481
  for (int i = 0; i < distros.size(); i++) {
    def distro = distros.get(i)
    builders["${distro}"] = {
      node('docker') {
        runContainer(distro, 'build')
      }
    }
  }
  return builders
}

properties([[$class: 'BuildDiscarderProperty', strategy: [$class: 'LogRotator', artifactDaysToKeepStr: '30', artifactNumToKeepStr: '', daysToKeepStr: '30', numToKeepStr: '']]]);

timestamps {
  node('docker') {
    stage('code-analysis') {
      runContainer('opensuse42.2', 'codeAnalysis')
    }
    stage('build') {
      throttle(['fet-build']) {
        parallel listBuilders(['centos7', 'debian8', 'opensuse42.2', 'ubuntu16.04'])
      }
    }
  }
}
