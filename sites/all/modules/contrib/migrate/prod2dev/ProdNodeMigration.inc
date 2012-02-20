<?php
/**
 * Created by JetBrains PhpStorm.
 * User: z3cka
 * Date: 2/20/12
 * Time: 1:26 AM
 * To change this template use File | Settings | File Templates.
 */

abstract class ProdToDev extends Migration {
	public function __construct() {
		parent::__construct();
		$this->team = array(
			new MigrateTeamMember('Casey Grzecka', 'c@sey.gr', t('DUG LV')),
		);
	}
}

class ProdNodeMigration extends ProdToDev {
	public function __construct() {
		parent::__construct();

		$this->description = t('Migrate Prod\'s nodes to dev');

		$source_fields = array(
			'nid' => t('Node ID of content'),
//			'title' => t('Title'),
		);

		$query = db_select('duglv'.'.node', 'n')
		  ->fields('n', array('nid', 'vid', 'type', 'language', 'title', 'uid', 'status', 'created', 'changed', 'comment', 'promote', 'sticky', 'tnid', 'translate'))
			->condition('n.type', 'page', '=');
//		$query->join('duglv'.'.node_revision', 'nr', 'n.vid = nr.vid');
//		$query->addField('nr', 'body');
//		$query->addField('nr', 'teaser');
		// ^^ D6 example does not work
		$query->join('duglv'.'.field_data_body', 'fdb', 'n.nid = fdb.revision_id');
		$query->addField('fdb', 'body_value');

		$query->join('duglv'.'.users', 'u', 'n.uid = u.uid');
		$query->addField('u', 'name');
		$query->orderBy('n.changed');

		$this->source = new MigrateSourceSQL($query, $source_fields);
		$this->destination = new MigrateDestinationNode('page');

		$this->map = new MigrateSQLMap($this->machineName,
			array(
				'nid' => array(
					'type' => 'int',
					'unsigned' => TRUE,
					'not null' => TRUE,
					'description' => 'Unique Node ID',
					'alias' => 'n',
				)
			),
			MigrateDestinationNode::getKeySchema()
		);

		// Mappings
		$this->addFieldMapping('title', 'title');
		$this->addFieldMapping('is_new')->defaultValue(TRUE);
		$this->addFieldMapping('uid', 'uid');
		$this->addFieldMapping('revision')->defaultValue(TRUE);
		$this->addFieldMapping('revision_uid', 'uid');
		$this->addFieldMapping('created', 'created');
		$this->addFieldMapping('changed', 'changed');
		$this->addFieldMapping('status', 'status');
		$this->addFieldMapping('promote', 'promote');
		$this->addFieldMapping('sticky', 'sticky');
		$this->addFieldMapping('comment', 'comment');
		$this->addFieldMapping('language')->defaultValue('und');

		$this->addFieldMapping('path')->issueGroup(t('DNM'));
//		$this->addFieldMapping('pathauto_perform_alias')->defaultValue('1');

//		$body_arguments = MigrateTextFieldHandler::arguments(NULL, filter_default_format());
//		$this->addFieldMapping('field_content_body', 'body')
//			->arguments($body_arguments);
		// ^^ D6 version won't work for body
		$this->addFieldMapping('body_value', 'body');

//		$body_arguments = MigrateTextFieldHandler::arguments(NULL, filter_default_format());
//		$this->addFieldMapping('field_content_teaser', 'teaser')
//			->arguments($body_arguments);

		$this->addFieldMapping(NULL, 'name');
		$this->addFieldMapping(NULL, 'vid');
		$this->addFieldMapping(NULL, 'type');
		$this->addFieldMapping(NULL, 'language');
//		$this->addFieldMapping(NULL, 'moderate');
		$this->addFieldMapping(NULL, 'tnid');
		$this->addFieldMapping(NULL, 'translate');
	}
}

//class NodeCommentMigration extends