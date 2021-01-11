#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/prctl.h>
#define SUCCESS 0
#define ERR_ILL_RET 8

static int set_value(int simd_vl) {
  int ret;
  ret = prctl(PR_SVE_SET_VL, (simd_vl / 8) | PR_SVE_VL_INHERIT);
  if(ret < 0)
    return ERR_ILL_RET;

  return SUCCESS;
}

static int get_value(int *simd_vl) {
  int ret;
  ret = prctl(PR_SVE_GET_VL);
  if(ret < 0)
    return ERR_ILL_RET;
  *simd_vl = (ret & PR_SVE_VL_LEN_MASK) * 8;
  return SUCCESS;
}

int main(int argc, char ** argv)
{
  int simdvl, simd_vl = 512, err = SUCCESS, ret = 0;

  if(argc < 3) {
    fprintf(stderr, "setvlrun <vector length> <command and args>\n");
    exit(EXIT_FAILURE);
  }
  simdvl = atol(argv[1]);
  argv += 1;
  argc -= 1;
  //fprintf(stderr, "try to set simd vlen %d\n",simdvl);
  
  ret = set_value(simdvl);
  if(ret!=0) {
    fprintf(stderr, " *** failed to call simd set syscall vlen %d\n",simdvl);
    exit(EXIT_FAILURE);
  }
  ret = get_value(&simd_vl);
  if(ret!=0) {
    fprintf(stderr, " *** failed to call simd get syscall vlen %d\n",simdvl);
    exit(EXIT_FAILURE);
  }
  if(simdvl != simd_vl) {
    fprintf(stderr, " *** failed to set simd vlen %d\n",simdvl);
    exit(EXIT_FAILURE);
  }
  fprintf(stderr, "simd vl is set to %d\n",simd_vl);

  argv += 1;
  argc -= 1;

  if(argc > 0) {
    ret = execvp(*argv, argv);
  }
  return ret;
}
