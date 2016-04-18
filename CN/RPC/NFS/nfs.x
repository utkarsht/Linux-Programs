struct buf
{
	unsigned int size;
	char buffer[1024];	
};

struct file_args
{
	char filename[128];
	int offset;
	unsigned int size;
};

struct file_write
{
	struct buf data;
	struct file_args file_data;
};

program nfs_rpc 
{
	version nfs 
	{
		buf nfs_ls() = 1;
		int nfs_create(file_args) = 2;
		int nfs_rm(file_args) = 3;
		buf nfs_cd(file_args) = 4;
		buf read(file_args) = 5;
		int write(file_write) = 6;
		int mkdir(file_args) = 7;	
		int rmdir(file_args) = 8;		
	} = 1;
} = 0x2fffffff;
