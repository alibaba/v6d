#pragma once
#include "vineyard_fs.h"
#include "msg_mgr.h"

struct vineyard_sb_info {
	struct mutex s_lock;
	struct inode *vineyard_inode;

	spinlock_t inode_hash_lock;
	// struct hlist_head inode_hashtable[VINEYARD_HASHSIZE];
	struct list_head inode_list_head;
};

struct vineyard_inode_info {
    struct inode        vfs_inode;
    uint64_t            obj_id;
    enum OBJECT_TYPE    obj_type;
    struct list_head    inode_list_node;
};

#define VINEYARD_SB_INFO(x) ((struct vineyard_sb_info *)x->s_fs_info)

// inode.c
uint64_t get_next_vineyard_ino(void);
struct inode *vineyard_fs_build_inode(struct super_block *sb, const char *name);
struct vineyard_inode_info *get_vineyard_inode_info(struct inode *inode);

// file.c
void vineyard_fs_init_file_inode(struct inode *inode);

// dir.c
void vineyard_fs_init_dir_inode(struct inode *inode);