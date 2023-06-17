[<img src="https://i.imgur.com/hn4cNER.png" width="50%">](https://www.youtube.com/watch?v=48zevrg9ixQ "유튜브 이동")
# 스톤에이지 Cygwin 기반 서버 프로젝트
###### 해당 프로젝트는 @sutreamer에 의해 시작 되었으며 리눅스에서의 환경 구성이 복잡하여 시작되게 되었습니다.
###### 프로젝트 시작은 2023-06-17에 시작 되었습니다.
## 셋팅법
###### Mariadb 13.3.14가 필수적으로 필요합니다.
###### 다른 버전의 MariaDB도 2023.06.17 기준으론 지원이 되지만 향후 업데이트 이후에 지원하지 않을 수 있으므로 
###### 해당 버전을 권장합니다.
###### URL은 하단에 있습니다.
###### https://mariadb.org/download/?t=mariadb&o=true&p=mariadb&r=10.3.14&os=windows&cpu=x86_64&pkg=msi 

## 빌드방법
### SAAC 빌드

###### SAAC의 경우 해당 dll들이 필수적으로 필요합니다.
###### 다만 cygwin이 설치된 상태에서 빌드하려 할 경우 해당 dll로인해 빌드 충돌이 발생할 수 있습니다.

###### cygcrypto-1.1.dll,cygiconv-2.dll,cygmariadb-3.dll,cygssl-1.1.dll,cygwin1.dll,cygz.dll

###### 충돌을 방지하기 위하여 dlls 폴더에 적어놓은 6개의 dll파일들은 dlls로 이동 후
###### 빌드를 한 후 다시 dlls 폴더에서 원래 있던 경로로 이동 시켜줍니다.

### GMSV 빌드

###### GMSV 빌드의 경우는 cygwin1.dll 하나만 필수적으로 필요합니다.
###### saac와 동일하게 gmsv 폴더안에 dlls폴더가 있으며 같은 방식으로 진행 해주시면 됩니다.

