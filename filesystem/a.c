#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>


void list_cur_file_stat(const char *file_name);

int main(int argc, const char *argv[])
{
//	printf("argc %d argv[0] %s argv[1] %s\n", argc, argv[0], argv[1]);
	if (argc < 2) {
		printf("less argument\n");
		return -1;
	}

	list_all_file(argv[1], 0);

	return 0;
}

/*
 *
 * path 的最后一个字符需要是 '/'
 * 
 * dep : 递归深度，外界调用传 0
 */
#define MAX_DEPTH 20 //最高递归深度
int list_all_file(const char *path, int dep)
{
	if (path == NULL)
		return -1;

	int depth = dep; //记录递归深度
	char visiable[MAX_DEPTH] = {0};
	char full_path[200] = {0};
	struct dirent *ptr;
	DIR *dir;

	dir = opendir(path);
	if (dir == NULL) {
		perror("opendir");
		return -1;
	}

	if (depth != 0)
		memset(visiable, '|', depth);

	while ((ptr = readdir(dir)) != NULL)
	{
		if (strlen(path) + strlen(ptr->d_name) + 1 >= sizeof(full_path)) {
			printf("path is too long , please give the path more space\n");
			return -1;
		}
		memset(full_path, 0, sizeof(full_path));
		memcpy(full_path, path, strlen(path));
		strcat(full_path, ptr->d_name);

		if (ptr->d_type == DT_DIR) {
			if ((0 == strcmp(ptr->d_name, ".")) || (0 == strcmp(ptr->d_name, "..")))
				continue;

			strcat(full_path, "/");
			printf("%-20s  >> %s\n", visiable, full_path);
			list_all_file(full_path, ++depth);
			printf("%-20s  << %s\n", visiable, full_path);
		} else if (ptr->d_type == DT_REG) {
			printf("file name : %s\n", ptr->d_name);
		} else {
			printf("other name : %s\n", ptr->d_name);
		}

		if (full_path[strlen(full_path) - 1] == '/')
			full_path[strlen(full_path) - 1] == 0;
		list_cur_file_stat(full_path);
	}

	closedir(dir);
}

void list_cur_file_stat(const char *file_name)
{
	if (file_name == NULL)
		return ;

	struct stat s;

	if (-1 == stat(file_name, &s)) {
		perror("get stat failed");
		return ;
	}

	printf("mode : %8o\n", s.st_mode);
	printf("uid  : %6d\n", s.st_uid);
	printf("gid  : %6d\n", s.st_gid);
}
