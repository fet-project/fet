#!/usr/bin/env groovy
def gitClone() {
  sh "rm -rf ci-build"
  checkout([$class: 'GitSCM', branches: [[name: '*/master']],
            doGenerateSubmoduleConfigurations: false,
            extensions: [[$class: 'RelativeTargetDirectory',
                          relativeTargetDir: 'ci-build']],
            submoduleCfg: [],
            userRemoteConfigs: [[url: 'https://github.com/fet-project/ci-build.git']]])
}

def runContainer(String distro, String step) {
  echo "Running ${step} for ${distro}..."
  def imageName = "fetproject/${distro}:latest"
  def dockerWorkspace = '/tmp/workspace'
  def jobNameValid = env.JOB_NAME.replace('_', '')
  def containerName = "${jobNameValid}-${env.BUILD_ID}-${distro}"
  try {
    echo "Starting cointainer..."
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

def transformIntoBuildStep(String distro) {
  return {
    node('docker') {
      stage(name: "build-${distro}") {
        checkout scm
        gitClone()
        runContainer(distro, 'build')
      }
    }
  }
}

node('docker') {
  stage(name: 'git-clone') {
    checkout scm
    gitClone()
  }
  stage(name: 'code-analysis') {
    runContainer('opensuse42.2', 'codeAnalysis')
  }
  lock(name: 'build', concurrency: 2) {
    // Thanks to https://cinhtau.net/wp/parallel-steps-with-jenkinsfile/
    def distros = ['centos7', 'debian8', 'opensuse42.2', 'ubuntu16.04' ]
    def stepsForParallel = [:]
    // I tried distros.each() but does not work due to JENKINS-26481
    for (int i = 0; i < distros.size(); i++) {
      def d = distros.get(i)
      stepsForParallel["${d}"] = transformIntoBuildStep(d)
    }
    parallel stepsForParallel
  }
}
