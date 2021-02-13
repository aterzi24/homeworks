#include "ext2.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BASE_OFFSET 1024 /* location of the superblock in the first group */

struct super_operations s_op;
struct inode_operations i_op;
struct file_operations f_op;

char fs_name[] = "ext2";

/* Implement functions in s_op, i_op, f_op here */

loffset_t my_llseek(struct file *f, loffset_t o, int whence)
{
    if(whence == SEEK_SET)
        f->f_pos = o;
    if(whence == SEEK_CUR)
        f->f_pos += o;
    if(whence == SEEK_END)
        f->f_pos = f->f_inode->i_size + o;
    return f->f_pos;
}

ssize_t my_read(struct file *f, char *buf, size_t len, loffset_t *o)
{
    struct inode *tmp = f->f_inode;
    unsigned long blsize = tmp->i_sb->s_blocksize;
    char tmp_str[blsize];
    f->f_pos = *o;
    size_t remain = len;
    while(f->f_pos < 12 * blsize)
    {
        int index = f->f_pos / blsize;
        int offset = f->f_pos % blsize;
        lseek(tmp->i_sb->s_type->file_descriptor, BASE_OFFSET+(tmp->i_block[index]-1)*blsize+offset, SEEK_SET);
        if(remain > blsize - offset)
        {
            read(tmp->i_sb->s_type->file_descriptor, (void *)tmp_str, blsize-offset);
            f->f_pos += blsize - offset;
            remain -= blsize - offset;
            strcat(buf, tmp_str);
        }
        else
        {
            read(tmp->i_sb->s_type->file_descriptor, (void *)tmp_str, remain);
            f->f_pos += remain;
            strcat(buf, tmp_str);
            return len;
        }
    }

    int p_size = blsize / sizeof(unsigned int);
    loffset_t relative_pos = f->f_pos - 12 * blsize;

    while(relative_pos < blsize * p_size)
    {
        int index = relative_pos / blsize;
        int offset = relative_pos % blsize;
        unsigned int pt1;
        lseek(tmp->i_sb->s_type->file_descriptor, BASE_OFFSET+(tmp->i_block[12]-1)*blsize+index*sizeof(unsigned int), SEEK_SET);
        read(tmp->i_sb->s_type->file_descriptor, (void *)&pt1, sizeof(unsigned int));
        lseek(tmp->i_sb->s_type->file_descriptor, BASE_OFFSET+(pt1-1)*blsize+offset, SEEK_SET);
        if(remain > blsize - offset)
        {
            read(tmp->i_sb->s_type->file_descriptor, (void *)tmp_str, blsize-offset);
            f->f_pos += blsize - offset;
            relative_pos += blsize - offset;
            remain -= blsize - offset;
            strcat(buf, tmp_str);
        }
        else
        {
            read(tmp->i_sb->s_type->file_descriptor, (void *)tmp_str, remain);
            f->f_pos += remain;
            strcat(buf, tmp_str);
            return len;
        }
    }

    relative_pos -= blsize * p_size;

    while(relative_pos < blsize * p_size * p_size)
    {
        int index = (relative_pos % (blsize * p_size)) / blsize;
        int offset = relative_pos % blsize;
        unsigned int pt1;
        lseek(tmp->i_sb->s_type->file_descriptor, BASE_OFFSET+(tmp->i_block[13]-1)*blsize + relative_pos/(blsize * p_size)*sizeof(unsigned int), SEEK_SET);
        read(tmp->i_sb->s_type->file_descriptor, (void *)&pt1, sizeof(unsigned int));
        lseek(tmp->i_sb->s_type->file_descriptor, BASE_OFFSET+(pt1-1)*blsize + index*sizeof(unsigned int), SEEK_SET);
        read(tmp->i_sb->s_type->file_descriptor, (void *)&pt1, sizeof(unsigned int));
        lseek(tmp->i_sb->s_type->file_descriptor, BASE_OFFSET+(pt1-1)*blsize+offset, SEEK_SET);
        if(remain > blsize - offset)
        {
            read(tmp->i_sb->s_type->file_descriptor, (void *)tmp_str, blsize-offset);
            f->f_pos += blsize - offset;
            relative_pos += blsize - offset;
            remain -= blsize - offset;
            strcat(buf, tmp_str);
        }
        else
        {
            read(tmp->i_sb->s_type->file_descriptor, (void *)tmp_str, remain);
            f->f_pos += remain;
            strcat(buf, tmp_str);
            return len;
        }
    }

    relative_pos -= blsize * p_size * p_size;

    while(relative_pos < blsize * p_size * p_size *p_size)
    {
        int index1 = relative_pos / (blsize * p_size * p_size);
        int index2 = (relative_pos % (blsize * p_size * p_size)) / (blsize * p_size);
        int index3 = (relative_pos % (blsize * p_size )) / blsize;
        int offset = relative_pos % blsize;
        unsigned int pt1;
        lseek(tmp->i_sb->s_type->file_descriptor, BASE_OFFSET+(tmp->i_block[14]-1)*blsize + index1*sizeof(unsigned int), SEEK_SET);
        read(tmp->i_sb->s_type->file_descriptor, (void *)&pt1, sizeof(unsigned int));
        lseek(tmp->i_sb->s_type->file_descriptor, BASE_OFFSET+(pt1-1)*blsize + index2*sizeof(unsigned int), SEEK_SET);
        read(tmp->i_sb->s_type->file_descriptor, (void *)&pt1, sizeof(unsigned int));
        lseek(tmp->i_sb->s_type->file_descriptor, BASE_OFFSET+(pt1-1)*blsize + index3*sizeof(unsigned int), SEEK_SET);
        read(tmp->i_sb->s_type->file_descriptor, (void *)&pt1, sizeof(unsigned int));
        lseek(tmp->i_sb->s_type->file_descriptor, BASE_OFFSET+(pt1-1)*blsize+offset, SEEK_SET);
        if(remain > blsize - offset)
        {
            read(tmp->i_sb->s_type->file_descriptor, (void *)tmp_str, blsize-offset);
            f->f_pos += blsize - offset;
            relative_pos += blsize - offset;
            remain -= blsize - offset;
            strcat(buf, tmp_str);
        }
        else
        {
            read(tmp->i_sb->s_type->file_descriptor, (void *)tmp_str, remain);
            f->f_pos += remain;
            strcat(buf, tmp_str);
            return len;
        }
    }
}

int my_open(struct inode *i, struct file *f)
{
    f->f_op = i->f_op;
    f->f_flags = i->i_flags;
    f->f_mode = S_IFREG;
    f->f_pos = 0;

    return 0;
}

int my_release(struct inode *i, struct file *f)
{
    free(f->f_path);
    f->f_path = NULL;
    f->f_inode = NULL;
    f->f_op = NULL;
    f->f_flags = 0;
    f->f_mode = 0;
    f->f_pos = 0;

    return 0;
}

struct super_block *my_get_superblock(struct file_system_type *fs)
{
    if(fs == NULL)
        return NULL;

    struct ext2_super_block *my_ext2_sb = malloc(sizeof(struct ext2_super_block));

    lseek(fs->file_descriptor, BASE_OFFSET, SEEK_SET);
    ssize_t read_bytes = read(fs->file_descriptor, (void *)my_ext2_sb, sizeof(struct ext2_super_block));
    lseek(fs->file_descriptor, 0, SEEK_SET);

    if(read_bytes == 0)
    {
        free(my_ext2_sb);
        return NULL;
    }

    struct super_block *my_sb = malloc(sizeof(struct super_block));

    my_sb->s_inodes_count = my_ext2_sb->s_inodes_count;
    my_sb->s_blocks_count = my_ext2_sb->s_blocks_count;
    my_sb->s_free_blocks_count = my_ext2_sb->s_free_blocks_count;
    my_sb->s_free_inodes_count = my_ext2_sb->s_free_inodes_count;
    my_sb->s_first_data_block = my_ext2_sb->s_first_data_block;
    my_sb->s_blocksize = 1024 << my_ext2_sb->s_log_block_size;
    my_sb->s_blocksize_bits = 10 + my_ext2_sb->s_log_block_size;
    my_sb->s_blocks_per_group = my_ext2_sb->s_blocks_per_group;
    my_sb->s_inodes_per_group = my_ext2_sb->s_inodes_per_group;
    my_sb->s_minor_rev_level = my_ext2_sb->s_minor_rev_level;
    my_sb->s_rev_level = my_ext2_sb->s_rev_level;
    my_sb->s_first_ino = my_ext2_sb->s_first_ino;
    my_sb->s_inode_size = my_ext2_sb->s_inode_size;
    my_sb->s_block_group_nr = my_ext2_sb->s_block_group_nr;

    unsigned long long num_point= my_sb->s_blocksize/sizeof(unsigned int);
    my_sb->s_maxbytes = my_sb->s_blocksize*(11 + (num_point*num_point*num_point*num_point-1)/(num_point-1));

    my_sb->s_type = fs;
    my_sb->s_op = &s_op;
    my_sb->s_flags = 0;
    my_sb->s_magic = my_ext2_sb->s_magic;

    my_sb->s_root = malloc(sizeof(struct dentry));
    my_sb->s_root->d_name = malloc(sizeof(char));
    strcpy(my_sb->s_root->d_name, ".");

    struct inode wanted;
    wanted.i_ino = 2;
    s_op.read_inode(&wanted);
    i_op.lookup(&wanted, my_sb->s_root);

    free(my_ext2_sb);
    return my_sb;
}

struct dentry *my_lookup(struct inode *i, struct dentry *dir)
{
    unsigned long blsize = i->i_sb->s_blocksize;
    loffset_t max_size = i->i_size;
    loffset_t cur_size = 0;
    struct dentry tmp;
    int ite, j;
    for( ite = 0 ; ite < 12 ; ite++ )
    {
        lseek(i->i_sb->s_type->file_descriptor, BASE_OFFSET+(i->i_block[ite]-1)*blsize, SEEK_SET);
        for( j = 0 ; j < blsize / sizeof(struct dentry) ; j++ )
        {
            read(i->i_sb->s_type->file_descriptor, (void *)&tmp, sizeof(struct dentry));
            cur_size += sizeof(struct dentry);
            if(cur_size > max_size)
                return NULL;
            if(strcmp(tmp.d_name, dir->d_name))
            {
                dir->d_flags = tmp.d_flags;
                dir->d_inode = tmp.d_inode;
                dir->d_parent = tmp.d_parent;
                dir->d_sb = tmp.d_sb;
                return dir;
            }
        }
    }
    //to be continued
}

int my_readlink(struct dentry *dir, char *buf, int len)
{
    if(S_ISLNK(dir->d_inode->i_mode) == 0)
        return -1;

    if(len > dir->d_inode->i_size)
        len = dir->d_inode->i_size;

    unsigned long blsize = dir->d_inode->i_sb->s_blocksize;
    char tmp[blsize];
    int remain = len, i;

    for( i = 0 ; i < 12 ; i++ )
    {
        lseek(dir->d_sb->s_type->file_descriptor, BASE_OFFSET+(dir->d_inode->i_block[i]-1)*blsize, SEEK_SET);
        if(remain < blsize)
        {
            read(dir->d_sb->s_type->file_descriptor, (void *)buf, remain);
            return len;
        }
        read(dir->d_sb->s_type->file_descriptor, (void *)tmp, blsize);
        strcat(buf, tmp);
        remain -= blsize;
    }
    return len;
}

int my_readdir(struct inode *i, filldir_t callback)
{
    unsigned long blsize = i->i_sb->s_blocksize;
    loffset_t max_size = i->i_size;
    loffset_t cur_size = 0;
    int dentry_count = 0;
    struct dentry tmp;
    int ite, j;
    for( ite = 0 ; ite < 12 ; ite++ )
    {
        lseek(i->i_sb->s_type->file_descriptor, BASE_OFFSET+(i->i_block[ite]-1)*blsize, SEEK_SET);
        for( j = 0 ; j < blsize / sizeof(struct dentry) ; j++ )
        {
            read(i->i_sb->s_type->file_descriptor, (void *)&tmp, sizeof(struct dentry));
            cur_size += sizeof(struct dentry);
            dentry_count++;
            if(cur_size > max_size)
                return dentry_count;
            callback(tmp.d_name, strlen(tmp.d_name), tmp.d_inode->i_ino);
        }
    }
    return dentry_count;
}

int my_getattr(struct dentry *dir, struct kstat *stats)
{
    stats->ino = dir->d_inode->i_ino;
    stats->mode = dir->d_inode->i_mode;
    stats->nlink = dir->d_inode->i_nlink;
    stats->uid = dir->d_inode->i_uid;
    stats->gid = dir->d_inode->i_gid;
    stats->size = dir->d_inode->i_size;
    stats->atime = dir->d_inode->i_atime;
    stats->mtime = dir->d_inode->i_mtime;
    stats->ctime = dir->d_inode->i_ctime;
    stats->blksize = dir->d_sb->s_blocksize;
    stats->blocks = dir->d_inode->i_blocks;

    return 0;
}

void my_read_inode(struct inode *i)
{
    unsigned long group_num = (i->i_ino - 1) / current_sb->s_inodes_per_group;
    unsigned long group_offs = (i->i_ino - 1) % current_sb->s_inodes_per_group;
    unsigned long group_size = (current_sb->s_first_data_block + current_sb->s_blocks_per_group) * current_sb->s_blocksize;
    unsigned long first_inode = ((current_sb->s_first_data_block - 1) * current_sb->s_blocksize - current_sb->s_inodes_per_group * current_sb->s_inode_size);
    lseek(current_sb->s_type->file_descriptor, BASE_OFFSET+group_size*group_num+first_inode+group_offs*current_sb->s_inode_size, SEEK_SET);
    struct ext2_inode tmp;
    read(current_sb->s_type->file_descriptor, (void *)&tmp, sizeof(struct ext2_inode));

    i->i_mode = tmp.i_mode;
    i->i_nlink = tmp.i_links_count;
    i->i_uid = tmp.i_uid;
    i->i_gid = tmp.i_gid;
    i->i_size = tmp.i_size;
    i->i_atime = tmp.i_atime;
    i->i_mtime = tmp.i_mtime;
    i->i_ctime = tmp.i_ctime;
    i->i_blocks = tmp.i_blocks;

    int ite;
    for( ite = 0 ; ite < 15 ; ite++ )
        i->i_block[ite] = tmp.i_block[ite];

    i->i_op = &i_op;
    i->f_op = &f_op;
    i->i_sb = current_sb;
    i->i_flags = tmp.i_flags;
}

int my_statfs(struct super_block *sb, struct kstatfs *stats)
{
    stats->name = sb->s_type->name;                       /* Name */
    stats->f_magic = sb->s_magic;           /* Magic number */
    stats->f_bsize = sb->s_blocksize;                     /* blocksize */
    stats->f_blocks = sb->s_blocks_count;                /* total blocks */
    stats->f_bfree = sb->s_free_blocks_count;                 /* free blocks */
    stats->f_inodes = sb->s_inodes_count;                /* total inodes */
    stats->f_finodes = sb->s_free_inodes_count;               /* free inodes */
    stats->f_inode_size = sb->s_inode_size;      /* inode size */
    stats->f_minor_rev_level = sb->s_minor_rev_level; /* minor revision level */
    stats->f_rev_level = sb->s_rev_level;         /* revision level */
    stats->f_namelen = strlen(stats->name);                   /* length of name */

    return 0;
}

struct file_system_type *initialize_ext2(const char *image_path) {
  /* fill super_operations s_op */
  /* fill inode_operations i_op */
  /* fill file_operations f_op */
  /* for example:
      s_op = (struct super_operations){
        .read_inode = your_read_inode_function,
        .statfs = your_statfs_function,
      };
  */
  f_op = (struct file_operations) {
      .llseek = my_llseek,
      .read = my_read,
      .open = my_open,
      .release = my_release,
  };

  i_op = (struct inode_operations) {
      .lookup = my_lookup,
      .readlink = my_readlink,
      .readdir = my_readdir,
      .getattr = my_getattr,
  };

  s_op = (struct super_operations) {
      .read_inode = my_read_inode,
      .statfs = my_statfs,
  };

  myfs.name = fs_name;
  myfs.file_descriptor = open(image_path, O_RDONLY);
  /* assign get_superblock function
     for example:
        myfs.get_superblock = your_get_superblock;
  */
  myfs.get_superblock = my_get_superblock;
  return &myfs;
}
