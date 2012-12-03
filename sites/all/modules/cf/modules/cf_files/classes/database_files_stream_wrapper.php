<?php

/**
 * @file
 * Provides cf_database_files DatabaseFilesStreamWrapper class.
 */

/**
 * @addtogroup cf_files
 * @{
 */

/**
 * Default files (database://) stream wrapper class.
 */
class DatabaseFilesStreamWrapper extends DrupalPrivateStreamWrapper {
  public function getDirectoryPath() {
    // this path will be dynamic and determined by the file itself.
    return 'profiles/mydistro/files';
  }
}

/**
 * @} End of '@addtogroup cf_database_files'.
 */
