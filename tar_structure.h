

/* Refrence wiki tar(computing);
 * https://en.wikipedia.org/wiki/Tar_(computing)
 * 
 * 
 * 
 * 
 */
struct tar_archive{
	char archive_name[50];
	struct file_tar_header **headers;
	int number_of_files_in_tar;
};

struct file_tar_header {
		int begin_adress;
        union {
            // OLD POSIX FORMAT OF TAR FILES
            struct {
                char name_of_file[100];             
                char mode_permissions[8];               
                char user_id[8];                
                char group_id[8];                
                char size_of_file[12];             
                char modification_time[12];             
                char checksum_for_header[8];              
                char link;                  
                char name_of_link_file[100];       
            };

            // UStar format (POSIX IEEE P1003.1)
            struct {
                char offset_of_old_posix[156];              
                char type_of_file;        
                char link_new[100];   
                char ustar_version[8];              
                char owner_name[32];             
                char owner_group_name[32];           
                char major[8];              
                char minor[8];             
                char filename_prefix[155];
            };
        };

        char block[512];                   
   

    struct file_tar * next;
};
