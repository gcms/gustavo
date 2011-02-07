var refreshForm = function() {};

/*!
 * Ext JS Library 3.3.0
 * Copyright(c) 2006-2010 Ext JS, Inc.
 * licensing@extjs.com
 * http://www.extjs.com/license
 */
Ext.onReady(function(){
    Ext.QuickTips.init();

    var Paciente = Ext.data.Record.create([{
        name: 'name',
        type: 'string'
    }]);


    // hideous function to generate employee data
    var genData = function(){
        var data = [];

        var form = document.getElementById('formulario');
        for (var i = 0; i < form.childNodes.length; i++) {
            var child = form.childNodes[i];
            if (child.name === undefined)
                continue;

            if (child.name.match('pacientes\[[0-9]+\]')) {
                data.push({ name: child.value });
                form.removeChild(child);
            }
        }

        return data;
    };


    var store = new Ext.data.GroupingStore({
        reader: new Ext.data.JsonReader({fields: Paciente}),
        data: genData()
    });

    var editor = new Ext.ux.grid.RowEditor({
        saveText: 'Alterar'
    });

    var grid = new Ext.grid.GridPanel({
        store: store,
        width: 380,
        region:'center',
        margins: '0 5 5 5',
        autoExpandColumn: 'name',
        plugins: [editor],
        view: new Ext.grid.GroupingView({
            markDirty: false
        }),
        tbar: [{
            iconCls: 'icon-user-add',
            text: 'Adicionar paciente',
            handler: function(){
                var e = new Paciente({
                    name: 'Novo paciente'
                });
                editor.stopEditing();
                store.insert(0, e);
                grid.getView().refresh();
                grid.getSelectionModel().selectRow(0);
                editor.startEditing(0);
            }
        },{
            ref: '../removeBtn',
            iconCls: 'icon-user-delete',
            text: 'Remover paciente',
            disabled: true,
            handler: function(){
                editor.stopEditing();
                var s = grid.getSelectionModel().getSelections();
                for(var i = 0, r; r = s[i]; i++){
                    store.remove(r);
                }
            }
        }],

        columns: [
        new Ext.grid.RowNumberer(),
        {
            id: 'name',
            header: 'Nome',
            dataIndex: 'name',
            width: 220,
            sortable: true,
            editor: {
                xtype: 'textfield',
                allowBlank: false
            }
        }]
    });


    var layout = new Ext.Panel({
        title: 'Pacientes',
        layout: 'border',
        layoutConfig: {
            columns: 1
        },
        width:380,
        height: 200,
        items: [grid]
    });
    layout.render('grid');

    grid.getSelectionModel().on('selectionchange', function(sm){
        grid.removeBtn.setDisabled(sm.getCount() < 1);
    });

    refreshForm = function() {
        var form = document.getElementById('formulario');

        for (var i = 0; i < store.getCount(); i++) {
            var data = store.getAt(i);

            var child = document.createElement('input');
            child.type = 'hidden';
            child.name = 'pacientes[' + i + ']';
            child.value = data.get('name');
            form.appendChild(child);
        }

        return false;
    };
});
